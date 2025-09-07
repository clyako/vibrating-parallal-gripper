import pyqtgraph as pg
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QWidget
from PyQt5.QtCore import Qt, QThread, pyqtSignal, QTimer
from collections import deque
import serial.tools.list_ports
import serial
import sys
import struct


def listports():
    ports = serial.tools.list_ports.comports()
    print("Available serial ports:")
    for port in ports:
        print(f"  {port.device}")


listports()

ser = serial.Serial("/dev/cu.usbmodem161524201", 115200, timeout=1)

# match Teensy: timestamp (I) + 8 floats (f)
num_bytes_to_read = 4 + 8 * 4  # = 36

teensy_send_data_rate = 1   # ms
time_window_size = 0.5      # s
y_axis_max = 2.0
num_data_points = int(time_window_size / (teensy_send_data_rate / 1000))
refresh_rate = 2            # ms


class ReadThread(QThread):
    update_plot_signal = pyqtSignal(float, float, float, float, float, float, float, float, float)

    def run(self):
        while True:
            try:
                if ser.in_waiting >= num_bytes_to_read:
                    data = ser.read(num_bytes_to_read)
                    if len(data) == num_bytes_to_read:
                        unpacked = struct.unpack('<Iffffffff', data)
                        timestamp = unpacked[0] / 1000.0
                        left_motor_1_target, left_motor_1_actual, left_motor_2_target, left_motor_2_actual, right_motor_1_target, right_motor_1_actual, right_motor_2_target, right_motor_2_actual = unpacked[
                            1:]
                        self.update_plot_signal.emit(timestamp, left_motor_1_target, left_motor_1_actual, left_motor_2_target,
                                                     left_motor_2_actual, right_motor_1_target, right_motor_1_actual, right_motor_2_target, right_motor_2_actual)
            except Exception:
                pass


class RealTimePlot(QWidget):
    def __init__(self):
        super().__init__()

        self.layout = QVBoxLayout()
        # Two plot widgets stacked vertically
        self.plot_widget_left = pg.PlotWidget()
        self.plot_widget_right = pg.PlotWidget()

        self.layout.addWidget(self.plot_widget_left)
        self.layout.addWidget(self.plot_widget_right)
        self.setLayout(self.layout)

        # Left motors plot
        self.plot_widget_left.setTitle("Left Motors")
        self.plot_widget_left.setLabel('left', 'Position (mm)')
        self.plot_widget_left.setLabel('bottom', 'Time (s)')
        self.plot_widget_left.setYRange(-y_axis_max, y_axis_max, padding=0.1)
        self.plot_widget_left.addLegend()

        # Right motors plot
        self.plot_widget_right.setTitle("Right Motors")
        self.plot_widget_right.setLabel('left', 'Position (mm)')
        self.plot_widget_right.setLabel('bottom', 'Time (s)')
        self.plot_widget_right.setYRange(-y_axis_max, y_axis_max, padding=0.1)
        self.plot_widget_right.addLegend()

        self.time_window = time_window_size

        # Buffers
        self.timestamps = deque(maxlen=num_data_points)
        self.left_motor_1_targets = deque(maxlen=num_data_points)
        self.left_motor_1_actuals = deque(maxlen=num_data_points)
        self.left_motor_2_targets = deque(maxlen=num_data_points)
        self.left_motor_2_actuals = deque(maxlen=num_data_points)
        self.right_motor_1_targets = deque(maxlen=num_data_points)
        self.right_motor_1_actuals = deque(maxlen=num_data_points)
        self.right_motor_2_targets = deque(maxlen=num_data_points)
        self.right_motor_2_actuals = deque(maxlen=num_data_points)

        # Lines (left)
        self.line_left_t1 = self.plot_widget_left.plot([], [], pen=pg.mkPen('y', width=2), name="Motor 1 Left Target")
        self.line_left_a1 = self.plot_widget_left.plot([], [], pen=pg.mkPen(
            'y', width=2, style=Qt.DashLine), name="Motor 1 Left Actual")
        self.line_left_t2 = self.plot_widget_left.plot([], [], pen=pg.mkPen('g', width=2), name="Motor 2 Left Target")
        self.line_left_a2 = self.plot_widget_left.plot([], [], pen=pg.mkPen(
            'g', width=2, style=Qt.DashLine), name="Motor 2 Left Actual")

        # Lines (right)
        self.line_right_t1 = self.plot_widget_right.plot(
            [], [], pen=pg.mkPen('c', width=2), name="Motor 1 Right Target")
        self.line_right_a1 = self.plot_widget_right.plot([], [], pen=pg.mkPen(
            'c', width=2, style=Qt.DashLine), name="Motor 1 Right Actual")
        self.line_right_t2 = self.plot_widget_right.plot(
            [], [], pen=pg.mkPen('m', width=2), name="Motor 2 Right Target")
        self.line_right_a2 = self.plot_widget_right.plot([], [], pen=pg.mkPen(
            'm', width=2, style=Qt.DashLine), name="Motor 2 Right Actual")

        # Worker thread
        self.worker_thread = ReadThread()
        self.worker_thread.update_plot_signal.connect(self.update_plot)
        self.worker_thread.start()

        # Timer to refresh plots
        self.refresh_timer = QTimer()
        self.refresh_timer.setInterval(refresh_rate)
        self.refresh_timer.timeout.connect(self.refresh_plot)
        self.refresh_timer.start()

    def update_plot(self, timestamp, left_motor_1_target, left_motor_1_actual, left_motor_2_target, left_motor_2_actual,
                    right_motor_1_target, right_motor_1_actual, right_motor_2_target, right_motor_2_actual):

        self.timestamps.append(timestamp)

        self.left_motor_1_targets.append(left_motor_1_target)
        self.left_motor_1_actuals.append(left_motor_1_actual)
        self.left_motor_2_targets.append(left_motor_2_target)
        self.left_motor_2_actuals.append(left_motor_2_actual)

        self.right_motor_1_targets.append(right_motor_1_target)
        self.right_motor_1_actuals.append(right_motor_1_actual)
        self.right_motor_2_targets.append(right_motor_2_target)
        self.right_motor_2_actuals.append(right_motor_2_actual)

    def refresh_plot(self):
        if not self.timestamps:
            return

        tmax = self.timestamps[-1]
        tmin = max(0, tmax - time_window_size)

        # Set fixed-length scrolling window
        self.plot_widget_left.setXRange(tmin, tmax)
        self.plot_widget_left.setXRange(tmin, tmax)

        # Left plot
        self.line_left_t1.setData(self.timestamps, self.left_motor_1_targets)
        self.line_left_a1.setData(self.timestamps, self.left_motor_1_actuals)
        self.line_left_t2.setData(self.timestamps, self.left_motor_2_targets)
        self.line_left_a2.setData(self.timestamps, self.left_motor_2_actuals)

        # Right plot
        self.line_right_t1.setData(self.timestamps, self.right_motor_1_targets)
        self.line_right_a1.setData(self.timestamps, self.right_motor_1_actuals)
        self.line_right_t2.setData(self.timestamps, self.right_motor_2_targets)
        self.line_right_a2.setData(self.timestamps, self.right_motor_2_actuals)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = RealTimePlot()
    window.show()
    sys.exit(app.exec_())
