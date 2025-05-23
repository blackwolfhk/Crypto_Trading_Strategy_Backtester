#!/usr/bin/env python3
"""
Visualize the backtesting results generated by the C++ backtester.
"""

import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import seaborn as sns

def load_strategy_data():
    """Load all CSV files containing strategy results."""
    strategy_files = glob.glob("*.csv")
    strategies = []
    
    for file in strategy_files:
        if file == "strategy_comparison.csv":
            continue
            
        strategy_name = file.replace('_', ' ').replace('.csv', '')
        
        try:
            df = pd.read_csv(file)
            df['Date'] = pd.to_datetime(df['Date'])
            strategies.append((strategy_name, df))
            print(f"Loaded {len(df)} records for {strategy_name}")
        except Exception as e:
            print(f"Error loading {file}: {e}")
    
    return strategies

def plot_comparison(strategies):
    """Plot a comparison of all strategy equity curves."""
    plt.figure(figsize=(12, 8))
    
    for name, df in strategies:
        plt.plot(df['Date'], df['Equity'], label=name)
    
    plt.title('Strategy Performance Comparison')
    plt.xlabel('Date')
    plt.ylabel('Portfolio Value ($)')
    plt.legend()
    plt.grid(True)
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m'))
    plt.gca().xaxis.set_major_locator(mdates.MonthLocator(interval=3))
    plt.gcf().autofmt_xdate()
    plt.tight_layout()
    plt.savefig('strategy_comparison.png', dpi=300)
    print("Saved strategy comparison chart to strategy_comparison.png")

def plot_individual_strategies(strategies):
    """Plot detailed charts for each strategy."""
    for name, df in strategies:
        # Create a figure with two subplots
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), gridspec_kw={'height_ratios': [3, 1]})
        
        # Top plot: Price and equity
        ax1.set_title(f'{name} Performance')
        
        # Plot Bitcoin price
        line1, = ax1.plot(df['Date'], df['Close'], color='blue', alpha=0.5, label='BTC Price')
        ax1.set_ylabel('BTC Price ($)')
        
        # Plot equity on a secondary y-axis
        ax3 = ax1.twinx()
        line2, = ax3.plot(df['Date'], df['Equity'], color='green', label='Portfolio Value')
        ax3.set_ylabel('Portfolio Value ($)')
        
        # Add a legend
        ax1.legend(handles=[line1, line2], loc='upper left')
        ax1.grid(True)
        
        # Bottom plot: Drawdown
        df['Peak'] = df['Equity'].cummax()
        df['Drawdown'] = (df['Equity'] / df['Peak'] - 1) * 100
        
        ax2.fill_between(df['Date'], df['Drawdown'], 0, color='red', alpha=0.3)
        ax2.set_ylabel('Drawdown (%)')
        ax2.set_xlabel('Date')
        ax2.grid(True)
        
        # Format dates
        for ax in [ax1, ax2]:
            ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m'))
            ax.xaxis.set_major_locator(mdates.MonthLocator(interval=2))
        
        plt.tight_layout()
        filename = f"{name.replace(' ', '_')}_performance.png"
        plt.savefig(filename, dpi=300)
        print(f"Saved {name} chart to {filename}")
        plt.close(fig)

def plot_performance_metrics():
    """Plot a comparison of performance metrics."""
    try:
        df = pd.read_csv("strategy_comparison.csv")
        
        metrics = ['Total Return', 'Annual Return', 'Sharpe Ratio', 'Max Drawdown', 'Win Rate']
        
        plt.figure(figsize=(15, 10))
        
        for i, metric in enumerate(metrics, 1):
            plt.subplot(2, 3, i)
            
            # Create bar chart
            sns.barplot(x='Strategy', y=metric, data=df)
            
            plt.title(metric)
            plt.xticks(rotation=45, ha='right')
            plt.tight_layout()
        
        plt.savefig('performance_metrics.png', dpi=300)
        print("Saved performance metrics chart to performance_metrics.png")
        
    except Exception as e:
        print(f"Error plotting performance metrics: {e}")

if __name__ == "__main__":
    print("Loading strategy data...")
    strategies = load_strategy_data()
    
    if not strategies:
        print("No strategy data found. Make sure to run the backtester first.")
        exit(1)
    
    print("\nGenerating charts...")
    plot_comparison(strategies)
    plot_individual_strategies(strategies)
    plot_performance_metrics()
    
    print("\nVisualization complete. All charts have been saved.")