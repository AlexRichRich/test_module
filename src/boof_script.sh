#!/ bin / bash

ls - li / sys / bus / platform / devices / |
    awk '{print "[" $8, $7, $9 "]", $10, $1, $12}' | column - t |
    sed '1d' >> / proc / read_dev_boof
