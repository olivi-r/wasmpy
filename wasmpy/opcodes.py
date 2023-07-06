import json
import os

opcodes = {}
with open(os.path.splitext(__file__)[0] + ".json") as fp:
    for group in json.load(fp):
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
