import json
import os

opcodes = {}
with open(os.path.splitext(__file__)[0] + ".json") as fp:
    data = json.load(fp)
    consumes = data["consumes"]
    for group in data["opcodes"]:
        opcodes.update(
            dict(
                zip(
                    group["instructions"],
                    (
                        i + group["offset"]
                        for i in range(len(group["instructions"]))
                    ),
                )
            )
        )
