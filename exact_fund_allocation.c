// Exact Fund Allocation Checker - Subset Sum (Dynamic Programming)
#include <stdio.h>

#define MAX 100
#define MAX_SUM 1000

int n;
int amount[MAX];
int dp[MAX+1][MAX_SUM+1];
int selected[MAX];

// Function to take input: number of items and their fund amounts
void enterAmounts()
{
    int i;
    printf("Enter number of items: ");
    fflush(stdout);
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter amount for item %d: ", i + 1);
        fflush(stdout);
        scanf("%d", &amount[i]);
    }

    printf("\nFund amounts entered successfully.\n");
}

// Function to display all entered fund amounts
void displayAmounts()
{
    int i;
    printf("\nAvailable Fund Amounts:\n");
    printf("Item\tAmount\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\t%d\n", i + 1, amount[i]);
    }
}

// Function to print the DP table (only if small enough to fit on screen)
void displayTable(int target)
{
    int i, s;

    if (n > 10 || target > 30)
    {
        printf("\nDP table hidden (too large to display).\n");
        return;
    }

    printf("\nDP Table (1 = achievable, 0 = not achievable):\n");
    printf("Items\\Sum");
    for (s = 0; s <= target; s++)
    {
        printf("\t%d", s);
    }
    printf("\n");

    for (i = 0; i <= n; i++)
    {
        printf("%d", i);
        for (s = 0; s <= target; s++)
        {
            printf("\t%d", dp[i][s]);
        }
        printf("\n");
    }
}

// Function to check if a subset of amounts exactly matches the target,
// build the DP table using include/exclude, and backtrack to find one subset.
void checkExactAllocation()
{
    int target, i, s, remaining, selectedCount, total;

    if (n == 0)
    {
        printf("\nEnter fund amounts first.\n");
        return;
    }

    printf("Enter target fund amount: ");
    fflush(stdout);
    scanf("%d", &target);

    total = 0;
    for (i = 0; i < n; i++)
    {
        total += amount[i];
    }
    printf("\nTotal of all available amounts: %d\n", total);

    // Base case: sum of 0 is always achievable with 0 items
    for (i = 0; i <= n; i++)
    {
        dp[i][0] = 1;
    }
    for (s = 1; s <= target; s++)
    {
        dp[0][s] = 0;
    }

    // Fill table using exclude / include
    for (i = 1; i <= n; i++)
    {
        for (s = 0; s <= target; s++)
        {
            dp[i][s] = dp[i-1][s];   // exclude current item

            if (amount[i-1] <= s && dp[i-1][s-amount[i-1]] == 1)
            {
                dp[i][s] = 1;        // include current item
            }
        }
    }

    if (dp[n][target] == 1)
    {
        printf("Exact allocation is possible.\n");

        // Backtrack to find one matching subset
        remaining = target;
        selectedCount = 0;
        for (i = n; i > 0 && remaining > 0; i--)
        {
            if (dp[i-1][remaining] == 0)
            {
                selected[selectedCount] = i;
                selectedCount++;
                remaining -= amount[i-1];
            }
        }

        if (selectedCount == 0)
        {
            printf("Matching subset: empty subset (target is 0).\n");
        }
        else
        {
            printf("One matching subset:\n");
            printf("Item\tAmount\n");
            for (i = selectedCount - 1; i >= 0; i--)
            {
                printf("%d\t%d\n", selected[i], amount[selected[i]-1]);
            }
        }
    }
    else
    {
        printf("Exact allocation is not possible for target %d.\n", target);
    }

    displayTable(target);
}

// Main Function: menu-driven, repeats until user chooses Exit
int main()
{
    int choice;

    do
    {
        printf("\n----- Exact Fund Allocation Checker -----\n");
        printf("1. Enter Fund Amounts\n");
        printf("2. Display Fund Amounts\n");
        printf("3. Check Exact Allocation\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 1)
        {
            enterAmounts();
        }
        else if (choice == 2)
        {
            displayAmounts();
        }
        else if (choice == 3)
        {
            checkExactAllocation();
        }
        else if (choice == 4)
        {
            printf("\nExiting program.\n");
        }
        else
        {
            printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 4);

    return 0;
}
