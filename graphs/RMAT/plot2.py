import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a DataFrame
df = pd.read_csv('results.csv')
# Extract degree from the 'File' column
df['Degree'] = df['File'].str.split('_').str[1]

# Convert 'Degree' to numeric (assuming it's a numeric value)
df['Degree'] = pd.to_numeric(df['Degree'], errors='coerce')

# Get unique degrees
unique_degrees = df['Degree'].unique()

# Calculate the number of rows and columns for the subplot grid
num_rows = int(len(unique_degrees) / 2) + len(unique_degrees) % 2  # Adjust the division factor as needed
num_cols = 2

# Create square-shaped subplots
fig, axs = plt.subplots(num_rows, num_cols, sharex=False, sharey=False, figsize=(10, 10))

# Flatten the axs array if needed
axs = axs.flatten()

# Iterate over each subplot and check if there is a corresponding degree
for i, ax in enumerate(axs):
    if i < len(unique_degrees):
        degree = unique_degrees[i]
        # Filter data for the current degree
        degree_data = df[df['Degree'] == degree]

        # Iterate over each combination of degree and threads and plot on the subplot
        for threads, group in degree_data.groupby('Threads'):
            ax.plot(
                group['V'].to_numpy(),
                group['MeanMatchingTime'].to_numpy(),
                label=f'Threads {threads}'
            )

        # Set labels and title for the subplot
        ax.set_title(f'Degree {degree}')
        ax.set_xlabel('V')
        ax.set_ylabel('MeanMatchingTime')

        # Add legend for the subplot
        ax.legend(title='Threads')

# Show the plot
plt.tight_layout()

# Save the figure
plt.savefig('your_output_figure.png')
# Save the figure
plt.savefig('your_output_figure.png')
