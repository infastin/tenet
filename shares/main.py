import numpy as np
import pandas as pd

import scipy.stats as sts
import matplotlib.pyplot as plt
import seaborn as sns

plt.style.use('ggplot')  # стиль для графиков

df_price = pd.read_csv('price.tsv', sep='\t')
df_price.head()

df_inf = pd.read_csv('information.tsv', sep='\t')
df_inf.head()

n_sp = 0
n_founded = 0

for _, row in df_inf.iterrows():
    if row['S&P-500'] and row['Founded'] <= 1900:
        n_founded += 1
    if row['NASDAQ'] and row['S&P-500'] and row['DJI']:
        n_sp += 1

assert n_sp < 10
assert n_founded < 100

df_inf.dropna(how='all', subset='GICS Sector', inplace=True)
n_sectors = df_inf['GICS Sector'].nunique()

assert n_sectors > 5
assert n_sectors < 15

top3_sectors = df_inf['GICS Sector'].value_counts().nlargest(3).keys()
n_top3 = 0

for _, row in df_inf.iterrows():
    if row['S&P-500'] and row['GICS Sector'] in top3_sectors:
        n_top3 += 1

assert n_top3 < 300
assert n_top3 > 200

n_ticker = 0

for _, row in df_inf.iterrows():
    if len(row['Symbol']) == 3:
        n_ticker += 1

assert n_ticker > 300

letter_count = df_inf['Symbol'].apply(len).value_counts()

first_letter = df_inf['Symbol'].apply(lambda sym: sym[0])
popular_letter = first_letter.value_counts().nlargest(1).keys()[0]

assert popular_letter != 'T'

tickers = df_inf['Symbol']
tickers10 = np.random.choice(tickers, 10, False)

assert tickers.size > 500

prices10 = df_price.filter(tickers10)
# prices10.plot(figsize=(12,7))
# prices10.hist(bins=25, density=True)

df_r = prices10.diff() / prices10.shift(1)
df_r.drop(index=0, inplace=True)

assert df_r.iloc[0,0] < 1

# df_r.plot(figsize=(12,7))
# df_r.hist(bins=25, density=True)
# df_r.plot.box(figsize=(12,7))

df_price_nd = df_price.drop(labels="Date", axis=1)

df_price_r = df_price_nd.diff() / df_price_nd.shift(1)
df_price_r.drop(index=0, inplace=True)

R = df_price_r.mean()
rtop_10 = R.nlargest(10)
rbottom_10 = R.nsmallest(10)

assert 'DXCM' in rtop_10
assert 'SLB' in rbottom_10

STD = df_price_r.std()
stdtop_10 = STD.nlargest(10)
stdbottom_10 = STD.nsmallest(10)

assert 'TSLA' in stdtop_10
assert 'PEP' in stdbottom_10

data = pd.DataFrame({'std': STD, 'r': R})

data = data.join(df_inf.set_index('Symbol')['GICS Sector'])
data.fillna('another sector')
data.head()

plt.figure(figsize=(15, 8))

sns.scatterplot(data=data, x='std', y='r', hue='GICS Sector')

tsla = data.loc['TSLA']
sns.regplot(x=[tsla['std']], y=[tsla['r']], scatter=True, fit_reg=False, 
        marker='x', color='black', scatter_kws={'s':100})

plt.ylim(-0.002, 0.005)
plt.xlim(0.01, 0.04)

plt.xlabel('Риск')
plt.ylabel('Доходность')
plt.title('Риск и доходность различных ценных бумаг');

df_agg = data.groupby('GICS Sector')
min_sector = df_agg.median()['std'].nsmallest(1).keys()[0]
max_sector = data['r'].nlargest(1).keys()[0]

assert min_sector == 'Utilities'

VaR = -data.quantile(0.95, axis=1)
VaRtop_10 = VaR.nsmallest(10).sort_index()
VaRbottom_10 = VaR.nlargest(10).sort_index()

assert 'TSLA' in VaRtop_10 
assert 'PG' in VaRbottom_10

var_tsla = -tsla.quantile(0.95)
es_tsla = -tsla

print(es_tsla)

assert np.abs(es_tsla + 0.073254619) < 1e-5
