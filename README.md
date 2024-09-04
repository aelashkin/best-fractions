# Fraction Search Program

This program searches for fractions that approximate a given target ratio (`c`) within a specified tolerance (`delta`). It supports two modes of operation and stops after finding the best 10 fractions or when the time limit is reached.

## Features
- **Two Modes**:
  - **Mode 1**: Stops after finding the first 10 fractions that meet the delta constraint.
  - **Mode 2**: Searches for fractions that minimize the sum of the numerator (`a`) and denominator (`b`) while satisfying the delta constraint.
  
- **Precision Handling**: The user can specify the precision (`delta`) for how close the fraction should approximate the target ratio (`c`).

- **Time Limit**: The search halts when the specified time limit is reached, even if fewer than 10 fractions are found.

## Example Program Output

```plaintext
Searching for fractions close to 3.14159 within a delta of 1e-05 and time limit of 5 seconds.
       a       b            a/b          delta
===============================================
    2862     911      3.1416026        0.00001
    2818     897      3.1415831        0.00001
    3217    1024      3.1416016       0.000009
    3173    1010      3.1415842       0.000008
    3572    1137      3.1416007       0.000008
    3528    1123      3.1415850       0.000008
    3927    1250      3.1416000       0.000007
    3883    1236      3.1415858       0.000007
    4238    1349      3.1415864       0.000006
     355     113      3.1415929       0.000000
```

This shows the numerators (`a`) and denominators (`b`) of fractions that approximate the target ratio (`3.14159`), along with the calculated fraction (`a/b`) and the difference (`delta`) from the target.

## Input
- **Target ratio** (`c`): The value that the fractions should approximate.
- **Delta**: Tolerance, how close the fraction needs to be to `c`.
- **Time limit**: Maximum time allowed for the search (in seconds).
- **Positive delta only**: Set to `1` to only consider fractions where the delta is positive (fraction is greater than `c`), or `0` to allow both positive and negative deltas.
- **Mode**:
  - `1`: Stops after finding 10 fractions that meet the delta.
  - `2`: Searches for fractions that minimize `a + b` while satisfying the delta condition.

## Usage

1. Clone the repository and compile the program:
   ```bash
   g++ -o fraction_search fraction_search.cpp
   ```

2. Run the program:
   ```bash
   ./fraction_search
   ```

3. Enter the required parameters when prompted:
   - Target ratio (`c`)
   - Tolerance (`delta`)
   - Time limit (in seconds)
   - Positive delta only (`1` for yes, `0` for no)
   - Mode (`1` or `2`)



## License

This project is licensed under the MIT License.