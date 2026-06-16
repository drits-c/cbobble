# cbobble

A tiny terminal bobblehead toy.

```
   ___         ___            ___
  (o o)       (o o)          (o o)
  /| |\       /| |\          /| |\
   | |         | |            | |
  _d b_       _d b_          _d b_
```

## Build

```bash
make
```

## Usage

```bash
cbobble                  # default bobblehead
cbobble -t cat           # bobbling cat
cbobble -t dog           # bobbling dog
cbobble -t owl           # bobbling owl
cbobble -r               # random character
cbobble -s 100           # faster (100ms per frame)
cbobble -n 5             # run 5 cycles then exit
cbobble -c               # disable colors
cbobble -l               # list characters
```

## Characters

| Name | Preview |
|------|---------|
| default | `(o o)` |
| cat | `(=^.^=)` |
| dog | `(o  o)` |
| owl | `{o,o}` |

## Install

```bash
sudo make install        # installs to /usr/local/bin
sudo make uninstall
```

## License

MIT
