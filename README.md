# Quantization

Benchmarks for bit-level integer quantization applied to matrix multiplication. Each element is stored as a small fixed-point value packed with several others into a single 32-bit integer, reducing memory usage. The programs measure how different bit widths trade off compression ratio against compute time.

## How it works

A 32-bit `int` can hold multiple quantized values side-by-side:

| Bits per value | Values per `int` |
|:--------------:|:----------------:|
| 2              | 16               |
| 4              |  8               |
| 5              |  6               |
| 6              |  5               |
| 8              |  4               |
| 16             |  2               |

The quantized matrix multiply unpacks pairs of packed values with a bitmask, ANDs them together (bitwise intersection), and ORs the result into the output element — accumulating across the inner (k) dimension.

`conversion.c` also demonstrates a full round-trip: float → 6-bit fixed-point → pack → unpack → float, followed by a standard float matrix multiply on the recovered values.

## Files

| File           | Description |
|----------------|-------------|
| `regular.c`    | Baseline integer matrix multiply (no quantization) |
| `2quant.c`     | 2-bit quantized multiply (16 values per int) |
| `4quant.c`     | 4-bit quantized multiply (8 values per int) |
| `5quant.c`     | 5-bit quantized multiply (6 values per int) |
| `6quant.c`     | 6-bit quantized multiply (5 values per int) |
| `8quant.c`     | 8-bit quantized multiply (4 values per int) |
| `16quant.c`    | 16-bit quantized multiply (2 values per int) |
| `conversion.c` | Float ↔ 6-bit fixed-point conversion + full float matmul |

## Fixed-point format (`conversion.c`)

6-bit representation: 1 sign bit (bit 5) + 5 fractional bits (bits 4–0).  
Representable range: −1.0 to +0.96875 in steps of 0.03125.

## Build

```
make
```

Requires GCC with C11 support (`-std=c11`). Clean with `make clean`.

## Usage

All programs read matrix dimensions from stdin. The quantization benchmarks (`2quant` through `16quant` and `regular`) take the dimensions of the *compressed* matrices (m, n, k):

```
echo "64 64 64" | ./8quant
```

`conversion` takes compressed dimensions first, then original dimensions (the original matrices are 5× larger in each packed dimension):

```
printf "13 13 13\n64 64 64\n" | ./conversion
```

## License

BSD 2-Clause — see [LICENSE](LICENSE).
