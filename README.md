# High Performance Stock Data Processor (C++)

A high-performance C++ application designed to process large stock market datasets stored in CSV format.

The program reads and analyzes financial records containing Open, High, Low and Close prices.  
The dataset used during testing contains over **34 million records**, requiring efficient file parsing and memory management.

## Features

- Efficient CSV file parsing
- Processing of large datasets (34M+ records)
- Optimized data handling using C++ STL
- Financial data analysis (Open / High / Low / Close values)
- Memory efficient processing

## Technologies

- C++
- STL (Standard Template Library)
- File I/O
- Algorithm optimization


## Dataset
The program processes a CSV dataset containing stock market records.

Each row contains:

Date: The date of the recorded trading data.
Ticker: The stock symbol of the company.
Open: Opening price of the stock on the trading day.
High: Highest price reached during the trading day.
Low: Lowest price reached during the trading day.
Close: Closing price of the stock on the trading day.
Volume: The total number of shares traded during the day.
Dividends: Cash dividends issued on the date, if applicable.
Stock Splits: Stock split factor for the date, if any split occurre

Source: Kaggle — “9000+ Tickers of Stock Market Data (Full History)”

## How it works

1. The program reads a large CSV file containing stock data.
2. Each line is parsed into structured values.
3. Algorithms process the dataset to extract statistics and insights.
4. Results are displayed in the terminal.

## Example Output

1973-05-30: 0.125497
1973-05-31: 0.125497
1973-06-01: 0.123754
Postoji
1962-01-16 GD open=0.000000 close=0.226591
Dividende dionice CAT na datum 1962-01-16: 0.01042
Top 10 dionica po volumenu na datum 1962-01-16:
MO 2592000
GD 1724000
KO 1113600
GE 278439
IBM 266730
MDU 1973-06-01 3.07165e-008
MDU 1973-05-30 3.13095e-008
MDU 1973-03-05 3.13095e-008
MDU 1973-05-31 3.14281e-008
MDU 1973-03-21 3.15467e-008
