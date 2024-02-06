import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a DataFrame
df = pd.read_csv('results.csv')
# Extract degree from the 'File' column
df['Degree'] = df['File'].str.split('_').str[1]

# Convert 'Degree' to numeric (assuming it's a numeric value)
df['Degree'] = pd.to_numeric(df['Degree'], errors='coerce')
# Filter data for V=5000000
df_v5000000 = df[df['V'] == 5000000]

# Get unique degrees
unique_degrees = df_v5000000['Degree'].unique()

# Create a single plot
fig, ax = plt.subplots(figsize=(20, 12))  # Double the figsize

line_thickness = 5  # Adjust as needed
# Iterate over each combination of threads and plot on the subplot
# Iterate over each combination of degree and threads and plot on the single plot
for threads, group in df_v5000000.groupby('Threads'):
    ax.errorbar(
        group['Degree'].to_numpy(),
        group['MeanMatchingTime'].to_numpy(),
        yerr=group['Stdev'].to_numpy(),
        label=f'Threads {threads}',
        fmt='-o',  # Specify the format of the data points
        linewidth = line_thickness,  # Adjust as needed
        capsize=5  # Specify the cap size for error bars
    )

# Set labels and title for the plot
#ax.set_title('d v.s. Mean Matching Time for d-regular graphs', fontsize=24)  # Double the fontsize
ax.set_xlabel('Degree', fontsize=32)  # Double the fontsize
ax.set_ylabel('Mean Matching Time', fontsize=32)  # Double the fontsize

# Double the tick label sizes
ax.tick_params(axis='both', which='major', labelsize=24)

# Add legend to the plot
ax.legend(fontsize=24)  # Double the legend fontsize

# Show the plot
plt.tight_layout()

# Save the figure
plt.savefig('reg_random_degree_v_match_time.png')
