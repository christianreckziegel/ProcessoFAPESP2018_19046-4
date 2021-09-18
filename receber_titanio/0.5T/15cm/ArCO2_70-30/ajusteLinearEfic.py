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
x = [2.0,2.3,2.5,2.8,3.0,3.3,3.5,3.8,4.0,4.3] #true energy
#x = [2.22886,2.43232,2.41161,2.84722,3.1277,3.37639,3.48972,3.57593,4.15633,3.94152,4.25797,4.68252,4.64935] #measured energy 
y0 = [0.773196,0.649485,0.636364,0.525253,0.561224,0.6,0.568421,0.683673,0.622449,0.555556] #porcentagem
incx = [0.,0.,0.,0.,0.,0.,0.,0.,0.,0.]
incy0 = [0.118888,0.105093,0.102559,0.0899575,0.0945558,0.100525,0.0968734,0.108378,0.101514,0.0934306] #incerteza porcentagem
#a partir de 4.5 não funcionou mais pelo mesmo problema do campo 1mT: 3 sigma ultrapassa os limites do histograma e a integral vira 100%
y = np.array(y0)
incy = np.array(incy0)

# visualizando os dados pelo gráfico

plt.figure() 
plt.errorbar(x, y, incy, incx,'og')
plt.title("Porcentagem de dados no intervalo de ajuste 3\u03C3 \n para ArCO2(70/30), B=0.5T, E=2GeV")
plt.xlabel("Energia (GeV)")
plt.ylabel("Eficiência de reconstrução")
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
plt.xlabel("Energia (GeV)")
plt.ylabel("Eficiência de reconstrução")
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
plt.title("Porcentagem de dados no intervalo de ajuste 3\u03C3 com ajuste \n para ArCO2(70/30), B=0.5T, Aresta=30cm")
plt.xlabel("Energia (GeV)")
plt.ylabel("Eficiência de reconstrução")
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

