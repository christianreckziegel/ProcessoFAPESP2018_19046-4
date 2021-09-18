import pandas as pd
import numpy as np
import matplotlib
from matplotlib import pyplot as plt

#ArCO2 (70/30)
#Size = 15cm
#E = 2GeV

#mais energias
x = [0.001,0.25,0.5,1.,2.,5.] #campo magnético
y0 = [0.,0.525773,0.773196,0.838384,0.938144,1.] #porcentagem
incx = [0.,0.,0.,0.,0.,0.]
incy0 = [0.,0.0909407,0.118888,0.124773,0.136912,0.143592]
#não estou conseguindo coletar parâmetros de ajuste sempre, sendo necessário obter manualmente

y = np.array(y0)
incy = np.array(incy0)

yRes = np.array(x)

# visualizando os dados pelo gráfico

plt.figure() 
plt.errorbar(x, y, incy, incx,'og')
plt.title("Porcentagem de dados no intervalo de ajuste 3\u03C3 \n para ArCO2(70/30), E=2GeV, Aresta=30cm")
plt.xlabel("B (T)")
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
plt.xlabel("B (T)")
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
plt.title("Porcentagem de dados no intervalo de ajuste 3\u03C3 com ajuste \n para ArCO2(70/30), E=2GeV, Aresta=30cm")
plt.xlabel("B (T)")
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

