
from os.path import join


REPLACE = (
    ("Derivative(q1(t), t)", "dq1"),
    ("Derivative(q2(t), t)", "dq2"),
    ("Derivative(q3(t), t)", "dq3"),
    ("Derivative(q1(t), (t, 2))", "ddq1"),
    ("Derivative(q2(t), (t, 2))", "ddq2"),
    ("Derivative(q3(t), (t, 2))", "ddq3"),
    ("q1(t)", "q1"),
    ("q2(t)", "q2"),
    ("q3(t)", "q3"),
)

REPLACE_EXPR = (
    ("q2 - q3", "q2-q3"),
    ("Eq(", ""),
    (" + ", "\n+ "),
    (" - ", "\n- "),
    (", ", "\n=\n"),
)


def print_expr(symbol):
    text = str(symbol)
    for args in REPLACE + REPLACE_EXPR:
        text = text.replace(*args)

    for i, char in enumerate(text):
        if char == "\n":
            if text[i+1] not in ("\n", "=", "-", "+"):
                text = text[:i] + "@" + text[i+1:]

    text = text.replace("@", "\n+ ")

    if text[0] == "-":
        text = "- " + text[1:-1] + "\n\n"
    else:
        text = "+ " + text[:-1] + "\n\n"

    return text


def print_eq(symbol):
    text = str(symbol)
    for args in REPLACE:
        text = text.replace(*args)
    print(text)


def save_equations(equations, filename):

    with open(join("python", filename), "w+") as FILE:
        text = map(print_expr, equations)
        FILE.writelines(text)