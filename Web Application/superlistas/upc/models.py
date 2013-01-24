# coding=ISO-8859-1

from django.db import models
from django.contrib.auth.models import User

class Usuario(User):
    user = models.OneToOneField(User)
    key = models.CharField(unique=True, max_length=20)
    
    def __unicode__(self):
        return u"%s (%s)" %(self.user.username, self.key)

class Familia(models.Model):
    """docstring for Familia"""
    nombre = models.CharField(blank=True, max_length=100)
    descripcion = models.CharField(blank=True, max_length=100)
    def __unicode__(self):
        return u"%s (%s)" %(self.descripcion, self.nombre)
    
#class ProductoManager(models.Manager):
#    def get_by_natural_key(self, fam, nombre, precio):
#        return self.get(fam=fam, nombre=nombre, precio=precio)

class Producto(models.Model):
    """Modelo principal del producto"""
    nombre = models.CharField(max_length=100)
    upc = models.CharField(primary_key=True, max_length=20)
    departamento = models.CharField(blank=True, max_length=100)
    linea = models.CharField(blank=True, max_length=100)
    precio = models.CharField(blank=True, max_length=100, default="0")
    img_url = models.CharField(blank=True, max_length=100)
    fam = models.ForeignKey(Familia)
    class Admin:
        list_display = ('',)
        search_fields = ('',)

    def __unicode__(self):
        return u"%s" %self.nombre
        
    def get_nombre_chido(self):
        palabras = self.nombre.split(' ')
        lpalabras = []
        feas = ['con', 'sin', 'sobre', 'desde', 'para', 'por', 'sobre']
        for palabra in palabras:
            if len(palabra) > 2 and palabra.lower() not in feas:
                palabra = palabra[0].capitalize() + palabra[1:].lower()
            else:
                palabra = palabra.lower()
            lpalabras.append(palabra)
        return ' '.join(lpalabras)
        
    #Al serializar el objeto, con este metodo en ves de recibir una id, recibes fam, nombre y precio
    def natural_key(self):
        nom = self.get_nombre_chido()
        return (self.upc, self.fam.nombre, nom, self.precio)

# class fam(object):
#	"""docstring for fam"""
#	def __init__(self, arg):
#		super(fam, self).__init__()
#		self.arg = arg
        
        
class Lista(models.Model):
    nombre = models.CharField(max_length=100)
    id = models.IntegerField(primary_key=True)
    usuario = models.ForeignKey(Usuario)
    fecha = models.DateField(blank=True)
    comentario = models.TextField(blank=True)
    def __unicode__(self):
        return u'%s' % self.id
    
class Seleccionado(models.Model):
    lista = models.ForeignKey(Lista)
    producto = models.ForeignKey(Producto)
    cantidad = models.IntegerField(blank=True, default=1)
    comentario = models.TextField(blank=True)
    desplegado = models.BooleanField(default=False)
    
    def __unicode__(self):
        return u'[%d] %s (%d)' % (self.lista.id, self.producto.nombre, self.cantidad)
    
    def get_total_dinero(self):
        if self.producto.precio and self.producto.precio != '0':
            return '$%.2f' % (float(self.producto.precio) * self.cantidad)
        else:
            return 'N/A'
            
    def get_precio_dinero(self):
        if self.producto.precio and self.producto.precio != '0':
            return '$%.2f' % float(self.producto.precio)
        else:
            return 'N/A'
        
    def get_nombre_chido(self):
        palabras = self.producto.nombre.split(' ')
        lpalabras = []
        feas = ['con', 'sin', 'sobre', 'desde', 'para', 'por', 'sobre']
        for palabra in palabras:
            if len(palabra) > 2 and palabra.lower() not in feas:
                palabra = palabra[0].capitalize() + palabra[1:].lower()
            else:
                palabra = palabra.lower()
            lpalabras.append(palabra)
            
        return ' '.join(lpalabras)
    
