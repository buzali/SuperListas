$(document).ready(function() {
    // Ordenar los productos por nombre (2a columna)
    //$("#tablaProductos").tablesorter({ sortList: [[1,0]] });
    Dajaxice.setup({'default_exception_callback': function(){ console.log('DAJAXICE - ERROR') }});
    
    idlista = $('.lista').attr('id');

    fade = 100;
    refresh = 100;
    actualizarTotal();
    //Agregar botones de + y -
    var incdel = '<div class="inc">+</div><div class="dec" >-</div>';
    $('.cant').append(incdel);
    $('.dec, .inc').hide();
       
    //Agregar boton de delete
    var delBtn = '<button href="#" rel="twipsy" title="Seguro?!" class="btn small danger delbtn">X</button>';
    $('.descripcion').append(delBtn);
    $('.delbtn').hide();
    $('.delbtn').twipsy({
        placement: 'left',
    });

//    $('.prod').mouseenter(function() {
//        alert('HEY');
//        var modifiers = '.delbtn,';
//        var val = $(this).find('.num').html();
//        
//        if(val > 1)
//            modifiers += '.dec,';
//        if(val < 99)
//            modifiers += '.inc,';
//        $(this).find(modifiers).fadeIn(fade);
//
//            
//    }).mouseleave(function(){
//        $('.container').removeClass('unselectable'); //Poder seleccionar la lista
//        $(this).find('.dec, .inc, .delbtn').fadeOut(fade);
//    });
    
    $('.prod').live('mouseenter', function() {
        var modifiers = '.delbtn,';
        var val = $(this).find('.num').html();
        
        if(val > 1)
            modifiers += '.dec,';
        if(val < 99)
            modifiers += '.inc,';
        $(this).find(modifiers).fadeIn(fade);      
    }).live('mouseleave', function(){
        $('.container').removeClass('unselectable'); //Poder seleccionar la lista
        $(this).find('.dec, .inc, .delbtn').fadeOut(fade);
    });

    
    
    $(".inc").live("click", function(){
        $('.container').addClass('unselectable');// Porque los + y - son texto, deshabilitar la selección de texto y que no se haga todo caco
        modificarCantidad(1,$(this),true);
    });
    
    $(".dec").live("click", function(){
        $('.container').addClass('unselectable'); 
        modificarCantidad(-1,$(this),true);
    });
    
    $(".delbtn").live("click", function(){
        //ARE YOU SURE?
        var producto = $(this).parent().parent();
        $(this).twipsy('hide');
        var fam = producto.attr('en');
        producto.fadeOut(fade).remove(); //Fade out y eliminarlo
        
        var upc = producto.attr('id')
        var idlista = $('.lista').attr('id');
        Dajaxice.upc.borrar_seleccionado(function(data){},{'idlista':idlista, 'upc':upc});
        actualizarTotal();
        checarFamilias(fam);
        
    });
    
    function checarFamilias(fam){
        var productosdefamilia = "[en='"+fam+"']";
        otros= $(productosdefamilia);
        if(!(otros.length > 0)){
            familia = "[familia='"+fam+"']";
            familia = $(familia);
            familia.fadeOut();
        }
        console.log(fam);
    }
    
    function modificarCantidad(val, origen, click){
        var num = parseInt($(origen).parent().find('.num').text());
        var hermano = $(origen).siblings().get(1); //Cuidado de agregar otros divs al de cantidad
        
        num += val;  
        if(num>0 && num<100){
            if(click){
                $(hermano).fadeIn(fade);
            }
            $(origen).parent().find('.num').text(num);
        }
        if(num==1 || num==99)
            $(origen).fadeOut(fade); //Ocultar el signito opuesto
        
        var precio = parseFloat($(origen).parent().parent().find('.subtotal').attr('unitario'));
        if(!isNaN(precio) && precio > 0){
            var subtotal = (precio*num).toFixed(2);
            $(origen).parent().parent().find('.subtotal').text('$'+subtotal);
        }
        
        
        //-+-+-+-+-++-+-+-+- Mandar AJAX
        var upc = $(origen).parent().parent().attr('id')
        if(click){
            Dajaxice.upc.modificar_cantidad(function(data){},{'idlista':idlista, 'upc':upc, 'val':val});
        }
        actualizarTotal();
    }
    
    function actualizarTotal(){
        var total = 0;
        var totalproductos = 0;
        $.each($('.prod'),function(index, content){
            producto = $(content);
            cantidad = parseInt(producto.find('.num').text());
            precio = parseFloat(producto.find('.subtotal').attr('unitario'));
            total += cantidad*precio;
            totalproductos += cantidad;
        });
        $('.total').text('$'+total.toFixed(2));
        $('.fila_total').find('.num').text(totalproductos);
       
    }
    
    var timer = setInterval(function(){
        //console.log(idlista);
        Dajaxice.upc.refrescar_lista(refrescarLista,{'idlista':idlista});
    }, refresh);    
    
    function refrescarLista(data){
        $.each(data,function(index, content){
            cant = content.fields.cantidad;
            upc = content.fields.producto[0];
            fam = content.fields.producto[1];
            nombre = content.fields.producto[2];
            precio = content.fields.producto[3];
            
            //Desplegar fila de familia si no existe
            familia = "[familia='"+fam+"']";
            familia = $(familia);
            familia.show();
            
            //Aumentar cantidad del producto si existe
            if(document.getElementById(upc)){
                prod = $('#'+upc);
                val = parseInt(cant) - parseInt($('#'+ upc).find('.num').text());
                modificarCantidad(val,prod.find('.inc'),false);
            }
            //Agregar producto si no existe
            else{
                productohtml = '<tr class="prod" id="'+upc+'" en="'+fam+'">\
                  <td class="cant">                           \
                    <div class="num">'
                      +cant+
                    '</div>                                    \
                  <div class="inc" style="display: none; ">+</div><div class="dec" style="display: none; ">-</div></td>                                        \
                  <td class="descripcion">'+nombre+'<button href="#" rel="twipsy" class="btn small danger delbtn" style="display: none; " data-original-title="Seguro?!">X</button></td>      \
                  <td class="precio">$'+precio+'</td>           \
                  <td class="subtotal" unitario="'+precio+'">$'+precio*cant+'</td>\
                </tr>'
                $(productohtml).insertAfter(familia).hide().fadeIn(fade);
            }
            
            console.log(familia);
        })      
    }
    
        
    //$("[familia='d_BEBES%20Y%20MATERNIDAD']").show()
});