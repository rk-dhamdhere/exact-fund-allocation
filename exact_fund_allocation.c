#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 100
#define MAX_TARGET 100000
#define INPUT_SIZE 128

int itemCount = 0;
int amounts[MAX_ITEMS];

// Read and validate one integer from the user.
int readInteger(const char *prompt, int minimum, int maximum, int *value)
{
    char input[INPUT_SIZE];
    char *end;
    long parsedValue;

    while (1)
    {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            return 0;
        }

        errno = 0;
        parsedValue = strtol(input, &end, 10);
        while (isspace((unsigned char)*end))
        {
            end++;
        }

        if (end == input || *end != '\0' || errno == ERANGE ||
            parsedValue < minimum || parsedValue > maximum)
        {
            printf("Invalid input. Enter an integer from %d to %d.\n", minimum, maximum);
            continue;
        }

        *value = (int)parsedValue;
        return 1;
    }
}

// Enter the available fund amounts.
int enterAmounts()
{
    int i;
    int newItemCount;
    char prompt[INPUT_SIZE];

    if (!readInteger("Enter number of items (1-100): ", 1, MAX_ITEMS, &newItemCount))
    {
        return 0;
    }

    for (i = 0; i < newItemCount; i++)
    {
        snprintf(prompt, sizeof(prompt), "Enter amount for item %d (non-negative integer): ", i + 1);
        if (!readInteger(prompt, 0, INT_MAX, &amounts[i]))
        {
            return 0;
        }
    }

    itemCount = newItemCount;
    printf("\nFund amounts entered successfully.\n");
    return 1;
}

// Display all entered fund amounts.
void displayAmounts()
{
    int i;

    if (itemCount == 0)
    {
        printf("\nEnter fund amounts first.\n");
        return;
    }

    printf("\nAvailable Fund Amounts:\n");
    printf("Item\tAmount\n");
    for (i = 0; i < itemCount; i++)
    {
        printf("%d\t%d\n", i + 1, amounts[i]);
    }
}

// Print the dynamic-programming table when its dimensions are small.
void displayTable(const unsigned char *table, size_t columns)
{
    int i;
    int sum;

    if (itemCount > 10)
    {
        printf("\nDP table hidden because there are more than 10 items.\n");
        return;
    }

    printf("\nDP Table (1 = achievable, 0 = not achievable):\n");
    printf("%9s", "Items\\Sum");
    for (sum = 0; sum < (int)columns; sum++)
    {
        printf("%4d", sum);
    }
    printf("\n");

    for (i = 0; i <= itemCount; i++)
    {
        printf("%9d", i);
        for (sum = 0; sum < (int)columns; sum++)
        {
            printf("%4d", table[(size_t)i * columns + (size_t)sum] != 0);
        }
        printf("\n");
    }
}

// Find and display one subset whose sum exactly matches the target.
int checkExactAllocation()
{
    int target;
    int i;
    int sum;
    int remaining;
    int selected[MAX_ITEMS];
    int selectedCount = 0;
    long long totalAvailable = 0;
    long long selectedTotal = 0;
    size_t rows;
    size_t columns;
    unsigned char *table;
    char prompt[INPUT_SIZE];

    if (itemCount == 0)
    {
        printf("\nEnter fund amounts first.\n");
        return 1;
    }

    for (i = 0; i < itemCount; i++)
    {
        totalAvailable += amounts[i];
    }

    snprintf(prompt, sizeof(prompt), "Enter target fund amount (0-%d): ", MAX_TARGET);
    if (!readInteger(prompt, 0, MAX_TARGET, &target))
    {
        return 0;
    }

    rows = (size_t)itemCount + 1;
    columns = (size_t)target + 1;
    if (rows > SIZE_MAX / columns)
    {
        printf("\nThe requested DP table is too large to represent.\n");
        return 1;
    }

    table = (unsigned char *)calloc(rows * columns, sizeof(*table));
    if (table == NULL)
    {
        printf("\nNot enough memory to create the DP table for this target.\n");
        return 1;
    }

    table[0] = 1;
    for (i = 1; i <= itemCount; i++)
    {
        for (sum = 0; sum <= target; sum++)
        {
            size_t currentCell = (size_t)i * columns + (size_t)sum;
            size_t previousRow = (size_t)(i - 1) * columns;

            table[currentCell] = table[previousRow + (size_t)sum];
            if (amounts[i - 1] <= sum &&
                table[previousRow + (size_t)(sum - amounts[i - 1])])
            {
                table[currentCell] = 1;
            }
        }
    }

    printf("\nTotal of all available amounts: %lld\n", totalAvailable);
    if (table[(size_t)itemCount * columns + (size_t)target])
    {
        remaining = target;
        for (i = itemCount; i > 0 && remaining > 0; i--)
        {
            size_t previousRow = (size_t)(i - 1) * columns;

            if (!table[previousRow + (size_t)remaining])
            {
                selected[selectedCount++] = i;
                remaining -= amounts[i - 1];
            }
        }

        printf("Exact allocation is possible.\n");
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
                int itemIndex = selected[i] - 1;
                printf("%d\t%d\n", selected[i], amounts[itemIndex]);
                selectedTotal += amounts[itemIndex];
            }
        }
        printf("Achieved total: %lld\n", selectedTotal);
    }
    else
    {
        printf("Exact allocation is not possible for target %d.\n", target);
    }

    if (target <= 30)
    {
        displayTable(table, columns);
    }
    else
    {
        printf("\nDP table hidden because the target is greater than 30.\n");
    }

    free(table);
    return 1;
}

// Main function: display the menu until the user chooses to exit.
int main()
{
    int choice;

    do
    {
        printf("\nExact Fund Allocation Checker\n");
        printf("1. Enter Fund Amounts\n");
        printf("2. Display Fund Amounts\n");
        printf("3. Check Exact Allocation\n");
        printf("4. Exit\n");

        if (!readInteger("Enter your choice: ", 1, 4, &choice))
        {
            break;
        }

        switch (choice)
        {
            case 1:
                if (!enterAmounts())
                {
                    return 0;
                }
                break;
            case 2:
                displayAmounts();
                break;
            case 3:
                if (!checkExactAllocation())
                {
                    return 0;
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
        }
    }
    while (choice != 4);

    return 0;
}