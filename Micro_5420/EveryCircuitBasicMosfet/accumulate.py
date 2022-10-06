from glob import glob

if __name__ == '__main__':
    problem_files = glob("Problem*.py")
    # print(type(problem_files))
    problem_files.sort()
    with open('Basic_Mosfets.md', 'w') as w:
        w.write("""
Calvin Passmore

A02107892

ECE 5420

# Basic Mosfets

I worked out all the math in python to facilitate changes my answers more easily.
Please see the attached python files and corresponding output.

""")
        for idx in range(len(problem_files)):
            p_file_name = f"Problem{idx + 1}.py"
            w.write(f"""

## {p_file_name.replace('.py', '')}

### Source Code

```
""")
            with open(p_file_name, 'r') as r:
                for line in r.readlines():
                    w.write(line)
            w.write("""\n```\n\n""")
            w.write("### Screenshot\n\n")
            w.write(f"![](SS{idx + 1}.png)\n")
            w.write("### Answers\n\n")
            with open(f"answers{idx + 1}.md", 'r') as r:
                for line in r.readlines():
                    w.write(line)
            w.write("\n-------------------------------")
            w.write("\n-------------------------------")
