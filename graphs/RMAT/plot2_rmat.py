import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a DataFrame
df = pd.read_csv('results.csv')

# Create a single plot
fig, ax = plt.subplots(figsize=(20, 12))  # Double the figsize
line_thickness = 5  # Adjust as needed
# Iterate over each combination of threads and plot on the subplot
for threads, group in df.groupby('Threads'):
    ax.errorbar(
        group['V'].to_numpy(),
        group['MeanMatchingTime'].to_numpy(),
        yerr=group['Stdev'].to_numpy(),  # Assuming 'Stdev' is the column containing uncertainty data
        label=f'Threads {threads}',
        fmt='-o',  # Specify the format of the data points
        linewidth = line_thickness,  # Adjust as needed
        capsize=10  # Double the cap size for error bars
    )

# Set labels and title for the plot
#ax.set_title('d v.s. Mean Matching Time for d-regular graphs', fontsize=24)  # Double the fontsize
ax.set_xlabel('V', fontsize=32)  # Double the fontsize
ax.set_ylabel('Mean Matching Time', fontsize=32)  # Double the fontsize

# Double the tick label sizes
ax.tick_params(axis='both', which='major', labelsize=24)

# Add legend to the plot
ax.legend(fontsize=24)  # Double the legend fontsize

# Show the plot
plt.tight_layout()

# Save the figure
plt.savefig('rmat_2.png')
