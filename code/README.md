# Process EHR DB 

---

## Extract age specific patients 

```
Program information:
  -h [ --help ]          print help message.
  -V [ --version ]       print version number

Usage:
  -D [ --dbfile ] arg    database file []
  -A [ --agemax ] arg    Max Age [5]
  -a [ --agemin ] arg    Min Age [0]
  -o [ --obfile ] arg    output file [out.dat]

```

### Usage:

```
./bin/extract_age_group -D /run/media/ishanu/D3/DXRXPX/tPATT/data/CASES_DX.txt -A 20 -o data/DX_age20.dat./bin/extract_age_group -D /run/media/ishanu/D3/DXRXPX/tPATT/data/CASES_DX.txt -A 20 -o data/DX_age20.dat

```

# Generate Time Series with Quantized Codes

```
Program information:
  -h [ --help ]                 print help message.
  -V [ --version ]              print version number

Usage:
  -P [ --phnfile ] arg          phenotype categoroes spec file []
  -F [ --fipsfile ] arg         FIPS spec file
  -I [ --intfile ] arg          Integrated file
  -f [ --fips ] arg             FIPS spec in command line
  -D [ --dxfile ] arg           dx file []

Output options:
  -w [ --resultdirectory ] arg  directory for string output files [./]
  -a [ --agemax ] arg           max age [5]
  -z [ --zeropref ] arg         zero pref. True/on/1: birth synced [1]
  -T [ --truncate ] arg         truncate at first occurrence []
  -X [ --exclude ] arg          exclude any occurrence []
  -N [ --necess ] arg           must occur []
  -t [ --timer ] arg            show timer [true]

```

## Usage

```
./bin/procdb -D ./data/DX_age20.dat -P ./phnfiles/phnfile -T 1 -N 2 -I int.dat -w TMPDIR -z 1 -a 20
```


