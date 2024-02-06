import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('results2.csv')

# Get unique file names
files = df['File'].unique()

# Create subplots in one row
fig, axs = plt.subplots(1, len(files), figsize=(15, 5), sharey=True)

# Iterate over each file and create a bar chart
for i, file in enumerate(files):
    sub_df = df[(df['File'] == file) & (df['Threads'].isin([1, 2, 4, 8]))]
    x_values = range(len(sub_df))
    axs[i].bar(x_values, sub_df['Mean Matching Time'], yerr=sub_df['Stdev'], capsize=5)
    axs[i].set_title(file, fontsize=20)  # Double the font size for the title
    axs[i].set_xlabel('Threads', fontsize=20)  # Set the x-axis label font size
    axs[i].set_ylabel('Mean Matching Time', fontsize=20)  # Set the y-axis label font size
    axs[i].tick_params(axis='both', labelsize=15)  # Set tick label font size
    axs[i].set_xticks(x_values)
    axs[i].set_xticklabels(sub_df['Threads'])

# Add a common y-axis label
#fig.text(0.04, 0.5, 'Mean Matching Time', va='center', rotation='vertical', fontsize=20)  # Double the font size
plt.savefig('empirical.png', bbox_inches='tight')

# Show the plots
plt.show()
