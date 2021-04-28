import pandas as pd
import matplotlib.pyplot as plt
import os

def shorten_filename(filename):
    return filename[:filename.find('_')]

criterias = ['makespan']

result_files = os.listdir('results')
results_dict = {criteria: {} for criteria in criterias}
shortened_filenames = []
for result_file in result_files:
    df = pd.read_csv('/'.join(('results', result_file)), delimiter='|')
    shortened_filename = shorten_filename(result_file)
    shortened_filenames.append(shortened_filename)
    for criteria in criterias:
        results_dict[criteria][shortened_filename] = df[criteria]
        results_dict[criteria][shortened_filename] = df[criteria]

for criteria in criterias:
    boxplot = pd.DataFrame.from_dict(results_dict[criteria]).boxplot(column=shortened_filenames)
    plt.xlabel('Algorithm')
    plt.ylabel(criteria)
    plt.show()
