# Exact Fund Allocation Mini Project

A menu-driven C program that checks whether a target fund amount can be formed exactly from a set of available amounts. It uses dynamic programming (the subset-sum approach) and displays one matching subset when one exists.

## Build and Run

With GCC installed, compile and run from this directory:

```sh
gcc exact_fund_allocation.c -o exact_fund_allocation
./exact_fund_allocation
```

On Windows, compile and run with:

```powershell
gcc exact_fund_allocation.c -o exact_fund_allocation.exe
.\exact_fund_allocation.exe
```

The program supports up to 100 amounts and targets from 0 to 100,000.
