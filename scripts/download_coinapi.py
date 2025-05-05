#!/usr/bin/env python3
import pandas as pd
import requests
from datetime import datetime, timedelta
import os

# Function to create sample Bitcoin data
def create_sample_data():
    print("Creating sample Bitcoin historical data...")
    
    # Create data directory if it doesn't exist
    if not os.path.exists('../data'):
        os.makedirs('../data')
    
    # Start with a base price and generate synthetic data
    start_date = datetime(2018, 1, 1)
    end_date = datetime.now()
    
    # Generate dates
    dates = []
    current_date = start_date
    while current_date <= end_date:
        if current_date.weekday() < 5:  # Only weekdays
            dates.append(current_date)
        current_date += timedelta(days=1)
    
    # Start with a base price
    base_price = 10000
    import random
    import numpy as np
    
    # Generate price data with a random walk
    prices = [base_price]
    for i in range(1, len(dates)):
        # Random price change with some momentum and volatility
        change_pct = np.random.normal(0, 0.03)  # 3% daily volatility
        new_price = prices[-1] * (1 + change_pct)
        prices.append(max(new_price, 100))  # Ensure price doesn't go too low
    
    # Create DataFrame
    df = pd.DataFrame({
        'Date': [d.strftime('%Y-%m-%d') for d in dates],
        'Open': prices,
        'High': [p * (1 + random.uniform(0, 0.02)) for p in prices],
        'Low': [p * (1 - random.uniform(0, 0.02)) for p in prices],
        'Close': [p * (1 + random.uniform(-0.01, 0.01)) for p in prices],
        'Volume': [random.randint(1000000, 10000000) for _ in prices]
    })
    
    # Save to CSV
    output_path = '../data/btc_historical.csv'
    df.to_csv(output_path, index=False)
    print(f"Created {len(df)} records of simulated Bitcoin data")
    print(f"Data saved to {output_path}")
    return True

if __name__ == "__main__":
    create_sample_data()
    print("\nNote: This is simulated data for testing purposes.")
    print("For real Bitcoin data, consider using the manual download method from Yahoo Finance.")