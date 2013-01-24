# coding=ISO-8859-1
from elaphe import barcode
import re

def generateBarcode(code):
#     if len(code) != 12:
#         print 'La longitud tiene que ser de 12 dígitos [te faltan '+str(12-len(code))+'], ya que el último es check digit'
#         return 0
    bc = barcode('ean13', code, options=dict(includetext=True), scale=2, margin=4)
    try:
        bc.save('barcodes/'+code+'.png')
        return 1
    except IOError:
        print "No fue posible guardar la imagen del código de barras"
        return -1

def checkdigit(upc):
    """Calcula el check bit del final. Los nones los multiplica por 3. Se suman todos y se saca el valor para llegar a la decena más cercana"""
    sum = 0
    for i in range(len(upc)):
        if i%2 == len(upc)%2:
            sum += int(upc[i])
        else:
            sum += int(upc[i])*3
    checkdigit = (10-sum%10)%10
    return str(checkdigit)
    
def clean_upc(upc):
    cleanupc = re.sub(r'^0+','',upc) # Quitar ceros del principio, superama style
    cleanupc = cleanupc[:-1] # Quitar check digit
    return cleanupc
    
