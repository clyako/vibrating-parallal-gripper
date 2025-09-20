import pandas as pd
import matplotlib.pyplot as plt


def get_quaid_position_curve(times):
    period_us = 1/20 * 1e6
    a_min = 0.8 * 9.81 / 1e9
    a_max = 20.0 * 9.81 / 1e9
    t1 = (a_max / (a_max + a_min)) * period_us / 2
    t2 = period_us - t1

    positions = []

    for t_seconds in times:
        t = t_seconds * 1e6
        periodic_t = t % (period_us)

        if (periodic_t <= t1):
            new_position = 0.5 * a_min * (periodic_t ** 2)
        elif (t1 < periodic_t and periodic_t <= t2):
            new_position = a_min * t1 * (periodic_t - 0.5 * t1) + a_max * \
                (t1 * periodic_t - 0.5 * (periodic_t ** 2) - 0.5 * (t1 ** 2))
        elif (periodic_t <= period_us):
            new_position = (a_min * (0.5 * (periodic_t ** 2) - period_us * periodic_t + 0.5 * period_us ** 2))

        positions.append(new_position)

    max_position = max(positions)
    position_offset = max_position / 2
    positions = [position - position_offset for position in positions]

    return positions


if __name__ == "__main__":
    plt.rcParams["text.usetex"] = True
    plt.rcParams['font.family'] = 'Times New Roman'
    plt.rcParams.update({'font.size': 32})

    # Load data
    data = pd.read_csv("tracking_data.csv")

    # Shift time so it starts at zero
    data["time"] = data["timestamp"] - data["timestamp"].iloc[0]

    # Keep only the desired window
    data = data[(data["time"] <= 0.45) & (data["time"] >= 0.05)]
    quaid_positions = get_quaid_position_curve(data["time"])

    linewidth = 3
    markersize = 60

    # Create subplots: 2 rows, 1 column
    fig, axes = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # --- Left Motor 1 ---
    axes[0].plot(
        data["time"], quaid_positions,
        linestyle="-", color="black", label="Target",
        linewidth=linewidth
    )
    axes[0].scatter(
        data["time"], data["left_motor_1_actual"], label="Actual",
        s=markersize, facecolors='none', edgecolors='#0076BA'
    )
    axes[0].set_ylabel("Motor 1\nPosition (mm)")
    axes[0].grid(True, which="both", axis='both', ls=":")

    # --- Left Motor 2 ---
    axes[1].plot(
        data["time"], quaid_positions,
        linestyle="-", color="black", label="Target",
        linewidth=linewidth
    )
    axes[1].scatter(
        data["time"], data["left_motor_2_actual"], label="Actual",
        s=markersize, facecolors='none', edgecolors='#0076BA'
    )
    axes[1].set_xlabel("Time (s)")
    axes[1].set_ylabel("Motor 2\nPosition (mm)")
    axes[1].grid(True, which="both", axis='both', ls=":")

    # --- Shared Legend ---
    handles, labels = axes[0].get_legend_handles_labels()
    fig.legend(
        handles, labels,
        loc="lower center", ncol=2,
        bbox_to_anchor=(0.5, -0.05)
    )

    plt.tight_layout()
    plt.subplots_adjust(bottom=0.15)  # make room for legend
    plt.show()
