import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
mpl.rc('axes.formatter', use_mathtext=True)
# Load the CSV file into a DataFrame
df = pd.read_csv('results.csv')
# Extract degree from the 'File' column
df['Degree'] = df['File'].str.split('_').str[1]

# Convert 'Degree' to numeric (assuming it's a numeric value)
df['Degree'] = pd.to_numeric(df['Degree'], errors='coerce')
df = df[df['Degree'] <= 7]
# Get unique degrees
unique_degrees = df['Degree'].unique()

# Calculate the number of rows and columns for the subplot grid
num_cols = 3
num_rows = int(len(unique_degrees) / num_cols) + len(unique_degrees) % num_cols  # Adjust the division factor as needed

# Create square-shaped subplots
fig, axs = plt.subplots(num_rows, num_cols, sharex=False, sharey=False, figsize=(16, 10))
# Flatten the axs array if needed
axs = axs.flatten()

# Iterate over each subplot and check if there is a corresponding degree
for i, ax in enumerate(axs):
    if i < len(unique_degrees):
        degree = unique_degrees[i]
        # Filter data for the current degree
        degree_data = df[df['Degree'] == degree]
        line_thickness = 2.5  # Adjust as needed
        # Iterate over each combination of degree and threads and plot on the subplot
        for threads, group in degree_data.groupby('Threads'):
            ax.errorbar(
                group['V'].to_numpy(),
                group['MeanMatchingTime'].to_numpy(),
                yerr=group['Stdev'].to_numpy(),
                label= f'Threads {threads}',
                fmt='-o',  # Specify the format of the data points
                linewidth = line_thickness,  # Adjust as needed
                capsize=5,  # Specify the cap size for error bars,
            )
        # Set labels and title for the subplot
        ax.set_title(f'Degree {degree}', fontsize=24)
        ax.set_xlabel('V', fontsize=20)
        ax.set_ylabel('Mean Matching Time', fontsize=20)
        ax.tick_params(axis='both', which='major', labelsize=16)
        ax.ticklabel_format(style = 'plain')

        # Add legend for the subplot
        #ax.legend(loc='upper left',fontsize=16)
handles, labels = plt.gca().get_legend_handles_labels()
by_label = dict(zip(labels, handles))
lgd = fig.legend(by_label.values(), by_label.keys(),loc='upper center', bbox_to_anchor=(0.5, 1.08), ncol=4, fontsize=20)
# Show the plot
plt.tight_layout()
# Save the figure
plt.savefig('scaling_d_regular.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
