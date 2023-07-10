import json, os


opcodes = {}
consumes = {}
signatures = {}
with open(os.path.splitext(__file__)[0] + ".json") as fp:
    data = json.load(fp)
    for group in data["opcodes"]:
        opcodes.update(
            dict(
                zip(
                    (i[0] for i in group["instructions"]),
                    (
                        i + group["offset"]
                        for i in range(len(group["instructions"]))
                    ),
                )
            )
        )

    for k, v in data["consumes"].items():
        consumes[opcodes[k]] = v

    for group in data["opcodes"]:
        signatures.update(
            dict(
                zip(
                    (opcodes[i[0]] for i in group["instructions"]),
                    (i[1:] for i in group["instructions"]),
                )
            )
        )
