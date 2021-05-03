#! /usr/bin/python3

import pandas as pd
import matplotlib.pyplot as plt
import os
import sys


ALGORITHM_NAMES = {
    'ArtificialBeeColony': 'ABC',
    'FirstComeFirstServe': 'FCFS',
    'GeneticAlgorithm': 'GA',
    'LongestRemainingTimeFirst': 'LRTF',
    'Random': 'RAND',
    'RoundRobin': 'RR',
    'ShortestElapsedTimeFirst': 'SETF',
    'ShortestRemainingTimeFirst': 'SRTF',
    'SimulatedAnnealing': 'SA'
}

POSSIBLE_ALGORITHM_TYPES = ('Offline', 'OnlineWithMigrationsAndPreemptions', 'Online')

CRITERIAS = ['makespan', 'flowtime']

DPI = 1200


def shorten_filename(filename):
    return filename[:filename.find('_')]


def split_filename(filename):
    for algorithm_name in POSSIBLE_ALGORITHM_TYPES:
        idx = filename.find(algorithm_name)
        if idx != -1:
            return filename[:idx], filename[idx:idx+len(algorithm_name)]


def prettify_algorithm_name(algorithm_name):
    return ALGORITHM_NAMES[algorithm_name]


def prettify_criteria(criteria):
    return criteria.capitalize()


def main():
    if len(sys.argv) == 2:
        print('Please provide path to results and alg type')
        exit(1)

    plt.figure(dpi=DPI)

    results_path = sys.argv[1]
    alg_type = sys.argv[2]

    algorithm_types = set()
    result_files = [filename for filename in os.listdir(results_path) if filename.endswith(alg_type)]
    results_dict = {algorithm_type: {criteria: {} for criteria in CRITERIAS} for algorithm_type in POSSIBLE_ALGORITHM_TYPES}
    prettified_alg_names = {}
    for result_file in result_files:
        df = pd.read_csv('/'.join((results_path, result_file)), delimiter='|')
        shortened_filename = shorten_filename(result_file)
        algorithm_name, algorithm_type = split_filename(result_file)
        algorithm_types.add(algorithm_type)
        prettified_alg_name = prettify_algorithm_name(algorithm_name)

        if algorithm_type not in prettified_alg_names:
            prettified_alg_names[algorithm_type] = []
        prettified_alg_names[algorithm_type].append(prettified_alg_name)
        for criteria in CRITERIAS:
            results_dict[algorithm_type][criteria][prettified_alg_name] = df[criteria]

    for algorithm_type in algorithm_types:
        for criteria in CRITERIAS:
            boxplot = pd.DataFrame.from_dict(results_dict[algorithm_type][criteria]).boxplot(column=prettified_alg_names[algorithm_type])
            plt.title(algorithm_type)
            plt.xlabel('Algorithm')
            plt.ylabel(prettify_criteria(criteria))
            plt.savefig('/'.join((results_path, criteria + algorithm_type)))


if __name__ == '__main__':
    main()
