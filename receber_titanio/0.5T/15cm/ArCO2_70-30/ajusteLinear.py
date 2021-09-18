import pandas as pd
import numpy as np
import matplotlib
from matplotlib import pyplot as plt

#B=0.5T
#Size=15cm
#ArCO2(70/30)

#menos energias
#x = [2.0,2.5,3.0,3.5,4.0] #energia esperada
#y0 = [2.229,2.412,3.128,3.49,4.156] #energia medida
#incx = [0.,0.,0.,0.,0.]
#incy0 = [0.4604,0.4803,0.6399,1.199,1.519]

#mais energias
x = [2.0,2.3,2.5,2.8,3.0,3.3,3.5,3.8,4.0,4.3,4.5,4.8,5.0] #energia esperada (do Geant4)
y0 = [2.22886,2.43232,2.41161,2.84722,3.1277,3.37639,3.48972,3.57593,4.15633,3.94152,4.25797,4.68252,4.64935] #energia medida
incx = [0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.]
incy0 = [0.0604213,0.0762864,0.114388,0.10896,0.122898,0.279289,0.243256,0.512137,0.264764,0.358352,0.477889,0.837213,1.02619] #erro do parâmetro média da gaussiana

y = np.array(y0)
incy = np.array(incy0)

yRes = np.array(x)

# visualizando os dados pelo gráfico

plt.figure() 
plt.errorbar(x, y, incy, incx,'og')
plt.title("Measured energy as a fuction of true energy \n for ArCO2(70/30), B=0.5T, Edge=30cm")
plt.xlabel("True energy (GeV)")
plt.ylabel("Measured energy (GeV)")
plt.show()

incy


#não precisa linearizar
#criando numpy array para x
xarray = np.array(x)
incxarray = np.array(incx)
#criando numpy array para y
yarray = np.array(y)

#minimos quadrados
#ajustando a reta (RLS) para obter uma estimativa boa de dy/dx
f_de_x = np.polyfit(x,yarray,1)
f_de_x
print("coeficiente angular: ", f_de_x[0])

#transferindo incerteza de x para y
print('incertezas em y (originais): ',incy)
incyarray = np.array(incy)
incytransf = np.sqrt(np.power(f_de_x[0]*incxarray,2)+np.power(incyarray,2)) 
np.set_printoptions(precision=3)
print('incertezas em y (novos val.):', incytransf)
#incremento % da transferência da incerteza de z para y
(incytransf/incyarray-1)*100

#plotando gráfico com as incertezas transferidas para a variável dependente (y), o 0 (zero) se refere a incertezas x
plt.errorbar(x, y, incytransf,0,'or')
plt.title("Incertezas transferidas")
plt.xlabel("True energy (GeV)")
plt.ylabel("Measured energy (GeV)")
print("imprimindo gráfico com as incertezas transferidas")
plt.show()

#matriz de covariancia
#fitpars = np.polyfit(z,yarray,1,full="False",cov='True')
np.set_printoptions(precision=9)
fitpars, cov_pars = np.polyfit(x,yarray,1,w=np.power(incytransf,-1),cov='True')
print(fitpars)
print("imprimindo matriz de covariânica...")
print(cov_pars)
cov_pars

#coeficientes da do grafico
p_0 = fitpars[0] #coef ang
p_1 = fitpars[1] #coef lin
s_p_0 = np.sqrt(cov_pars[0][0]) #incert p_0
s_p_1 = np.sqrt(cov_pars[1][1]) #incert p_1
rp_0 = np.round(p_0,3) #mudei precisão pra 4, era 2
rp_1 = np.round(p_1,2)
rs_p_0 = np.round(s_p_0,3) #mudei precisao pra 4, era 2
rs_p_1 = np.round(s_p_1,2)
print("Os coeficientes da reta são:")
print("coef. angular=",rp_0,"+-",rs_p_0)
print("coef. linear=",rp_1,"+-",rs_p_1)

#graficando o ajuste aos pontos
xline = xarray #não sei pq, mas nao aceitou 'x', pois numpy.float64 object cannot be interpreted as an integer
yline = fitpars[1] + fitpars[0] * xarray
#yline2 = 1.*xarray #y=x
plt.errorbar(x, y, incytransf,label="dados",fmt='or')
plt.title("Measured energy as a fuction of true energy with fitting \n for ArCO2(70/30), B=0.5T, Edge=30cm")
plt.xlabel("True energy (GeV)")
plt.ylabel("Measured energy (GeV)")
#incluindo a linha do ajuste linear, k=preto, '--' = linha tracejada
plt.plot(xline, yline, 'k--',label='ang. coef.='+str(rp_0)+'+-'+str(rs_p_0))
#mostrando a legenda no gráfico - o texto da legenda é definido por label='texto'
plt.legend()
print("imprimindo ajuste de reta...")
plt.show()

#qualidade do ajuste
#calulando chi^2_red
yhat = np.polyval(fitpars, x)
chi_squared = np.sum(((yhat - yarray)/incytransf) ** 2)
chi2_red = chi_squared/(len(x)-2)
chi2_red
print("chi quadrado reduzido = ", chi2_red)

#grafico de residuos
resid = (yhat - yarray) #original
#resid = (yRes - yarray) #y=x
plt.plot(x, resid, 'or')
plt.title("Gráfico de resíduos")
plt.xlabel("GeV")
plt.ylabel("residuos (GeV)")
plt.axhline(y=0, color='k', linestyle='-')
print("imprimindo gráfico de resíduos...")
plt.show()

