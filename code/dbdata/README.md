# Pushing & Pulling DB Files
---

## DB files are stored as git LFS objects

### But some are too big for even LFS because of the 2GB limit on upload

so, we break them up after compression.

To recreate DX_age20.dat, use:

```
 git pull
 git lfs pull
 cat DX20*part* > DX20.tgz
 tar -xzvf DX20.tgz
```