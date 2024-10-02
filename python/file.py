from tools import print_expr
from os.path import join
import pickle

from sympy import simplify

RESULTS = join("python", "results")
DATA = join("resources", "data")
PKL = ".pkl"


def save_equations(equations, filename):

    with open(join(RESULTS, filename), "w+") as FILE:
        text = map(print_expr, equations)
        FILE.writelines(text)


def save_result_str(expression, filename):
    
    with open(join(RESULTS, filename+PKL), "wb") as FILE:
        pickle.dump(str(expression), FILE)

        
def save_result(expression, filename):
    
    with open(join(RESULTS, filename+PKL), "wb") as FILE:
        pickle.dump(expression, FILE)


def load_result(filename):
    
    with open(join(RESULTS, filename+PKL), "rb") as FILE:
        expression = pickle.load(FILE)
    return simplify(expression)


def load_params(filename):
    data = {}

    with open(join(DATA, filename), "r") as FILE:
        lines = FILE.readlines()

        for line in lines:
            if line.strip() == "":
                continue
            name, value = line.split(" ")
            data[name] = float(value)
    
    return data

def write_tuning_params(data):

    with open(join(DATA, "tuning"), "w") as FILE:
        lines = []
        for index, value in enumerate(data):
            lines.append(f"K{index+1} {value}\n")
        FILE.writelines(lines)


if __name__ == "__main__":
    test = load_params("params")
    print(test)