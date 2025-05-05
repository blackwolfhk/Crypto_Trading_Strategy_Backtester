#!/usr/bin/env python3
import pandas as pd
import os
import yfinance as yf
from datetime import datetime, timedelta

def download_bitcoin_data():
    print("Downloading Bitcoin historical data...")
    
    # Create data directory if it doesn't exist
    if not os.path.exists('../data'):
        os.makedirs('../data')
    
    # Calculate date range (5 years of data)
    end_date = datetime.now()
    start_date = end_date - timedelta(days=5*365)
    
    try:
        # Download Bitcoin data from Yahoo Finance
        btc_data = yf.download('BTC-USD', 
                              start=start_date.strftime('%Y-%m-%d'),
                              end=end_date.strftime('%Y-%m-%d'),
                              progress=False)
        
        # Check if data was downloaded successfully
        if btc_data.empty or len(btc_data) < 10:
            print("Error: Downloaded data is empty or insufficient")
            return False
            
        # Format the data
        btc_data = btc_data.reset_index()
        btc_data = btc_data[['Date', 'Open', 'High', 'Low', 'Close', 'Volume']]
        btc_data['Date'] = btc_data['Date'].dt.strftime('%Y-%m-%d')
        
        # Save to CSV
        output_path = '../data/btc_historical.csv'
        btc_data.to_csv(output_path, index=False)
        print(f"Successfully downloaded {len(btc_data)} records of Bitcoin data")
        print(f"Data saved to {output_path}")
        return True
    
    except Exception as e:
        print(f"Error downloading data: {e}")
        return False

if __name__ == "__main__":
    # Make sure we have required packages
    try:
        import yfinance
    except ImportError:
        print("yfinance package not found. Installing it now...")
        import pip
        pip.main(['install', 'yfinance'])
        print("yfinance has been installed")
    
    # Download the data
    success = download_bitcoin_data()
    
    if not success:
        print("\nAlternative method: If the automatic download fails, you can:")
        print("1. Visit https://finance.yahoo.com/quote/BTC-USD/history")
        print("2. Download the historical data manually (set time period to max)")
        print("3. Save the file as 'btc_historical.csv' in the data directory")