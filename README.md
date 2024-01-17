# PCB_Motors
Code &amp; Firmware to generate continuous coil PCB motor designs


## Cloning the repo
Note that if you clone the repo, you will also need to intialize the submodules. You can do so as follows

```
git clone git@github.com:SaralTayal123/PCB_Motors.git
cd PCB_Motors

git submodule init
git submodule update
```

You will also need to delete/comment out line 6 of `kicad-coil-plugins/pcb_json.py` as follows

```
import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# from helpers import rotate ### Modify by commenting out ###
```
