import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy, JointState
from std_msgs.msg import Float64MultiArray

class JoyPositionController(Node):
    def __init__(self):
        super().__init__('joy_position_controller')

        # Subscriptions
        self.create_subscription(JointState, '/joint_states', self.joint_state_callback, 10)
        self.create_subscription(Joy, '/joy', self.joy_callback, 10)

        # Publisher naar ros2_control
        self.publisher = self.create_publisher(Float64MultiArray, '/position_controller/commands', 10)

        # Gewrichten in exacte volgorde van je controller
        self.arm_joints = ['joint_1', 'joint_2', 'joint_3', 'joint_4', 'joint_5', 'joint_6']

        # Limieten per gewricht: (min_rad, max_rad)
        self.joint_limits = {
            'joint_1': None,           # Geen beweging toegestaan
            'joint_2': (-1.000, 1.000),
            'joint_3': (0.000, 1.000),
            'joint_4': (-1.000, 1.000),
            'joint_5': (-0.500, 0.500),
            'joint_6': (-1.000, 1.000),
        }

        # Positietracking
        self.actual_positions = {}
        self.target_positions = {}
        self.initialized = False

        # Knop- & asdetectie
        self.prev_buttons = []
        self.prev_axes = []
        self.step_size = 0.05  # Stapgrootte per actie (~2.86 graden)

    def joint_state_callback(self, msg: JointState):
        """Update continu de actuele gewrichtsposities uit de hardware."""
        for name, pos in zip(msg.name, msg.position):
            if name in self.arm_joints:
                self.actual_positions[name] = pos

        # Bij de eerste meting: initialiseer target_positions op de actuele stand
        if not self.initialized and len(self.actual_positions) == len(self.arm_joints):
            for j in self.arm_joints:
                self.target_positions[j] = round(self.actual_positions[j], 3)
            self.initialized = True
            self.get_logger().info('Gewrichtsposities gesynchroniseerd met /joint_states!')

    def joy_callback(self, msg: Joy):
        """Verwerk joystick input en pas limieten toe."""
        if not self.initialized:
            self.get_logger().warn('Wachten op /joint_states voor eerste besturing...')
            return

        if not self.prev_buttons:
            self.prev_buttons = list(msg.buttons)
            self.prev_axes = list(msg.axes)
            return

        # ENABLE SWITCH: L1 (button index 4) moet ingedrukt zijn
        if len(msg.buttons) <= 4 or msg.buttons[4] != 1:
            self.prev_buttons = list(msg.buttons)
            self.prev_axes = list(msg.axes)
            return

        updated = False

        # --- JOINT 1: Geblokkeerd (geen actie) ---

        # --- JOINT 2: Linker Stick verticaal (index 1) ---
        lx_index = 1
        if len(msg.axes) > lx_index:
            val = msg.axes[lx_index]
            prev_val = self.prev_axes[lx_index] if len(self.prev_axes) > lx_index else 0.0
            if val > 0.5 and prev_val <= 0.5:
                self.target_positions['joint_2'] += self.step_size
                updated = True
            elif val < -0.5 and prev_val >= -0.5:
                self.target_positions['joint_2'] -= self.step_size
                updated = True

        # --- JOINT 3: Button 0 (Kruisje - CCW) & Button 2 (Vierkantje - CW) ---
        if len(msg.buttons) > 2:
            if msg.buttons[0] == 1 and self.prev_buttons[0] == 0:
                self.target_positions['joint_3'] -= self.step_size
                updated = True
            elif msg.buttons[2] == 1 and self.prev_buttons[2] == 0:
                self.target_positions['joint_3'] += self.step_size
                updated = True

        # --- JOINT 4: Linker Stick horizontaal (index 0) ---
        ly_index = 0
        if len(msg.axes) > ly_index:
            val = msg.axes[ly_index]
            prev_val = self.prev_axes[ly_index] if len(self.prev_axes) > ly_index else 0.0
            if val > 0.5 and prev_val <= 0.5:
                self.target_positions['joint_4'] += self.step_size
                updated = True
            elif val < -0.5 and prev_val >= -0.5:
                self.target_positions['joint_4'] -= self.step_size
                updated = True

        # --- JOINT 5: Rechter Stick Verticaal (index 4 in axes array) ---
        ry_index = 4
        if len(msg.axes) > ry_index:
            val = msg.axes[ry_index]
            prev_val = self.prev_axes[ry_index] if len(self.prev_axes) > ry_index else 0.0

            # Stick omhoog geduwd (> 0.5)
            if val > 0.5 and prev_val <= 0.5:
                self.target_positions['joint_5'] += self.step_size
                updated = True
            # Stick omlaag geduwd (< -0.5)
            elif val < -0.5 and prev_val >= -0.5:
                self.target_positions['joint_5'] -= self.step_size
                updated = True

        # --- JOINT 6: Rechter Stick Horizontaal (index 2 in axes array) ---
        # Als Rechter Stick Horizontaal op jouw controller index 3 gebruikt, verander 2 naar 3
        rx_index = 3
        if len(msg.axes) > rx_index:
            val = msg.axes[rx_index]
            prev_val = self.prev_axes[rx_index] if len(self.prev_axes) > rx_index else 0.0
            
            # Stick naar rechts geduwd (> 0.5)
            if val > 0.5 and prev_val <= 0.5:
                self.target_positions['joint_6'] += self.step_size
                updated = True
            # Stick naar links geduwd (< -0.5)
            elif val < -0.5 and prev_val >= -0.5:
                self.target_positions['joint_6'] -= self.step_size
                updated = True

        # Status opslaan voor volgende cycle
        self.prev_buttons = list(msg.buttons)
        self.prev_axes = list(msg.axes)

        if updated:
            # Clamping & Afronding op 3 decimalen
            cmd_array = []
            for j in self.arm_joints:
                limits = self.joint_limits[j]
                
                if limits is None:
                    # Gewricht is geblokkeerd, behoud actuele fysieke positie
                    val = self.actual_positions.get(j, 0.0)
                else:
                    min_lim, max_lim = limits
                    val = max(min_lim, min(max_lim, self.target_positions[j]))
                    self.target_positions[j] = val  # Update de geklemde waarde terug

                cmd_array.append(round(val, 3))

            # Publiceer commando
            msg_out = Float64MultiArray()
            msg_out.data = cmd_array
            self.publisher.publish(msg_out)
            self.get_logger().info(f'Gepubliceerd naar arm: {cmd_array}')

def main(args=None):
    rclpy.init(args=args)
    node = JoyPositionController()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
