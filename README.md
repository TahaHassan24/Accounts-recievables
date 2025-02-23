# Accounts-recievables
## Overview
The Accounts Management System is a command-line application that allows users to manage accounts, record transactions, and generate reports like the trial balance. The system handles account creation, adding sales, sales returns, and bank transactions, and provides account summaries and financial reports.

## Features
- Account Management: Create new accounts and view account details.
- Transaction Management: Add sales, sales returns, and bank transactions, automatically updating balances.
- Account View: View transaction history and account details.
- Trial Balance: View a summary of all accounts with debit and credit balances.

## Files
- accounts.dat: Stores account data and transactions.
- GeneralLedgerEntries.dat: Intended for general ledger entries (not fully utilized in the code)

## Functions
- addAccount(): Create a new account and optionally add transactions.
- addTransaction(int acc_id): Add a transaction to a specified account (Sales, Sales Return, Bank).
- ViewAccount(int id): View detailed transactions for a specific account.
- ViewAll(): List all accounts and view details for any selected account.
- ViewTrialBalance(): Display a trial balance with debit and credit totals.

## Usage
- Add an Account: Create a new account and optionally add transactions.
- Add a Transaction: Choose an account and transaction type (Sales, Sales Return, Bank) and input the values.
- View Account Details: View transaction history for a specific account.
- View Trial Balance: Display the trial balance for all accounts.

## Requirements
- C Compiler: GCC or compatible C compiler.
- File Access: Requires write access to accounts.dat and GeneralLedgerEntries.dat.
