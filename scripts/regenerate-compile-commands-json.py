import os, io, json

lst = []
for fn in [fn.replace(".cpp",".debug.o") for fn in os.listdir('../src') if fn.endswith(".cpp")]:
    lst.append({"command":"make","file":"build/src/{}".format(fn)})

outfile = "../compile_commands.json"
with open(outfile, "w", encoding="utf8") as fh:
    json.dump(lst, fh, indent=4)

print("Regenerated \"{}\" with:\n{}".format(outfile, json.dumps(lst)))
