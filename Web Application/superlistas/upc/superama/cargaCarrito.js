var artFlo = ['0750221956330', '0750221956332', '0750221956333', '0750221956334', '0750221956335', '0750221956363', '0750221956337', '0750221956338', '0750221956339', '0750221956340', '0750221956341', '0750221956342', '0750221956357', '0750221956358', '0750221956344', '0750221956345', '0750221956346', '0750221956347', '0750221956348', '0750221956349', '0750221956350', '0750221956351', '0750221956352', '0750221956353', '0750221956354', '0750221956355', '0750221956356', '0750221956359', '0750221956038', '0750221956037', '0750221956018', '0750221956331'];
simpleCart.cartHeaders = ["increment", "decrement", "Quantity_input", "name", "remove", "Price", "Total"];
var altura = 375;
$().ready(function () {
    $('#banner_container').cycle({
        fx: 'fade',
        timeout: 7000,
        cleartypeNoBg: true
    });
    var p = $(".simpleCart_items");
    p.height($(window).height() - altura);
    $('#contraer').click(function () {
        if ($("#texto").html() == "Contraer") {
            _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Contraer']);
            createCookie('estadocarrito', "contraido", 365);
            $("#texto").html("Desplegar");
            $("#imagenContraer").attr('src', 'images/cart/blue/cartToggleArrowDown.png');
        } else {
            _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Desplegar']);
            createCookie('estadocarrito', "expandido", 365);
            $("#texto").html("Contraer");
            $("#imagenContraer").attr('src', 'images/cart/blue/cartToggleArrowUp.png');
        }
        $('.simpleCart_items').animate({
            height: 'toggle'
        }, "fast", "swing", function () {});
    });
    $(window).resize(function () {
        var p = $(".simpleCart_items");
        p.height($(window).height() - altura);
    });
    $("#carritoCompleto").position();
    var $scrollingDiv = $("#carritoCompleto");
    $(window).scroll(function () {
        if ($(window).scrollTop() >= 159) {
            $scrollingDiv.stop().animate({
                "marginTop": ($(window).scrollTop() - 159) + "px"
            }, "slow");
        } else {
            $scrollingDiv.stop().animate({
                "marginTop": "0" + "px"
            }, "slow");
        }
    });
    var $dialog = $('#dialog-confirm').dialog({
        autoOpen: false,
        resizable: false,
        height: 140,
        modal: true,
        buttons: {
            "Cancelar": function () {
                $(this).dialog("close");
            },
            "Borrar art\u00edculos": function () {
                vaciaCarrito();
                $(this).dialog("close");
            }
        }
    });

    function checkLength(o, n, min, max) {
        if (o.val().length > max || o.val().length < min) {
            o.addClass("ui-state-error");
            updateTips("Length of " + n + " must be between " + min + " and " + max + ".");
            return false;
        } else {
            return true;
        }
    }
    $('#dialog-conviertecarrito').dialog({
        autoOpen: false,
        resizable: false,
        height: 140,
        modal: true,
        buttons: {
            "Cancelar": function () {
                $(this).dialog("close");
            },
            "Aceptar": function () {
                if ($("#nombrelista").val() != "") {
                    document.conviertecarrito.submit();
                    $(this).dialog("close");
                }
            }
        }
    });
    $('#dialog-conviertecarritoAnt').dialog({
        autoOpen: false,
        resizable: false,
        height: 140,
        modal: true,
        buttons: {
            "Cancelar": function () {
                $(this).dialog("close");
            },
            "Aceptar": function () {
                if ($("#nombrelista1").val() != "") {
                    document.conviertecarritoAnt.submit();
                    $(this).dialog("close");
                }
            }
        }
    });
    $('.botonConvierteEnLista').click(function () {
        _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Convierte en lista']);
        $('#dialog-conviertecarrito').dialog('open');
        return false;
    });
    $('.botonConvierteEnListaAnt').click(function () {
        _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Convierte en lista']);
        $('#dialog-conviertecarritoAnt').dialog('open');
        return false;
    });
    $('.cajaModal').click(function () {
        _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Vaciar']);
        $dialog.dialog('open');
        return false;
    });
    $('.botonProcedeCompra').click(function () {
        _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Procede a la compra']);
        window.location.href = "../procedeCompra.asp";
        return false;
    });
    $('.botonRevisaPedido').click(function () {
        _gaq.push(['_trackEvent', 'Carrito', 'Acciones', 'Revisa tu pedido']);
        window.location.href = "../revisa-tu-pedido.asp";
        return false;
    });
    $('#dialog-noadd').dialog({
        autoOpen: false,
        resizable: false,
        height: 140,
        modal: false,
        buttons: {
            "Aceptar": function () {
                $(this).dialog("close");
            }
        }
    });
    var superamaTotal = readCookie('superamaTotal');
    if (readCookie('estadocarrito')) {
        var estadocarrito = readCookie('estadocarrito');
        if (estadocarrito == "expandido") {
            desplegar();
        } else {
            contraer();
        }
    } else {
        contraer();
    }
});

function vaciaCarrito() {
    simpleCart.empty();
}

function contraer() {
    $('.simpleCart_items').hide();
    $("#texto").html("Desplegar");
    $("#imagenContraer").attr('src', 'images/cart/blue/cartToggleArrowDown.png');
}

function desplegar() {
    $('.simpleCart_items').show();
    $("#texto").html("Contraer");
    $("#imagenContraer").attr('src', 'images/cart/blue/cartToggleArrowUp.png');
}

function createCookie(name, value, days) {
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        var expires = "; expires=" + date.toGMTString();
    } else var expires = "";
    document.cookie = name + "=" + value + expires + "; path=/";
}

function readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1, c.length);
        if (c.indexOf(nameEQ) === 0) return c.substring(nameEQ.length, c.length);
    }
    return null;
}

function validaFloreria(upc) {
    var articuloFloreria = false;
    for (var i = 0; i < artFlo.length; i++) {
        if (upc == artFlo[i]) {
            articuloFloreria = true;
            break;
        }
    }
    var cookieNum = 0;
    var carritoFloreria = false;
    var carritoVacio = true;
    while (readCookie('simpleCart' + cookieNum)) {
        var data = unescape(readCookie('simpleCart' + cookieNum)).split('~~');
        for (var x = 0, xlen = data.length; x < xlen; x++) {
            carritoVacio = true;
            var info = data[x].split('||');
            var upcCarrito = info[2];
            for (var i = 0; i < artFlo.length; i++) {
                carritoVacio = false;
                if (upcCarrito == artFlo[i]) {
                    carritoFloreria = true;
                    break;
                }
            }
        }
        cookieNum++;
    }
    if (!carritoVacio) {
        if (articuloFloreria != carritoFloreria) {
            return false;
        }
    }
    if (articuloFloreria) {
        createCookie("flores", "true", 365);
    } else {
        createCookie("flores", "", -1);
    }
    return true;
}

function agregarCarrito(nombre, precio, upc, cantidad, pesable) {
    if (!validaFloreria(upc)) {
        $('#dialog-noadd').dialog('open');
        return;
    }
    javascript: simpleCart.add(nombre, precio, upc, 'name=' + upc, cantidad, pesable);
    var mensaje = $("#mensajeAgregar");
    mensaje.css("bottom", "-" + ($(window).scrollTop()) + "px").css("left", "0px");
    mensaje.fadeIn(800, function () {
        mensaje.fadeOut(800);
    });
}

function replaceAll(text, busca, reemplaza) {
    var buscavalue = busca.split(',');
    for (var i = 0; i <= buscavalue.length; i++) {
        while (text.toString().indexOf(buscavalue[i]) != -1) {
            text = text.toString().replace(buscavalue[i].toString(), reemplaza);
        }
    }
    return text;
}

function agregarCarritoUpdate(nombre, precio, upc, cantidad, pesable, comentario) {
    comentario = replaceAll(comentario, '|,~,\',-,=,"', '');
    var mesurable = pesable.substring(0, 1).toLowerCase();
    if (mesurable == 'c') {
        cantidad = parseInt(Math.ceil(cantidad), 10);
    }
    if (!validaFloreria(upc)) {
        $('#dialog-noadd').dialog('open');
        return;
    }
    if (validaBundleSushi(upc) > 0) {
        if (comentario == "") {
            comentario = ".";
        }
        window.open("BundleSushi.asp?Upc=" + upc + "&Cantidad=" + cantidad + "&Comentario=" + comentario, "BundleSushi", 'toolbar=0,height=550,width=750,top=150, left=200');
    } else {
        javascript: simpleCart.add(nombre, precio, upc, 'name=' + upc, cantidad, pesable, comentario);
        var mensaje = $("#mensajeAgregar");
        mensaje.css("bottom", "-" + ($(window).scrollTop()) + "px").css("left", "0px");
        mensaje.fadeIn(800, function () {
            mensaje.fadeOut(800);
        });
    }
}

function agregarCarritoBundle(nombre, precio, upc, cantidad, pesable, comentario) {
    comentario = replaceAll(comentario, '|,~,\',-,=,"', '');
    javascript: simpleCart.add(nombre, precio, upc, 'name=' + upc, cantidad, pesable, comentario);
}

function agregarLista(upc) {
    javascript: AddItemToList(upc);
    var mensaje = $("#response_form1");
    mensaje.css("bottom", "-" + ($(window).scrollTop()) + "px").css("left", "0px");
    mensaje.fadeIn(1000, function () {
        mensaje.fadeOut(1500);
    });
}

function agregarLista(upc) {
    javascript: AddItemToList(upc);
    var mensaje = $("#response_form1");
    mensaje.css("bottom", "-" + ($(window).scrollTop()) + "px").css("left", "0px");
    mensaje.fadeIn(1000, function () {
        mensaje.fadeOut(1500);
    });
}

function update(upc, value) {
    $.ajax({
        type: "POST",
        url: "../updateComment.asp",
        dataType: "application/x-www-form-urlencoded",
        data: "Upc=" + upc + "&Value=" + value,
        async: false,
        success: function (msg) {
            $("#fldCom").append(msg);
        }
    })
}

function validaBundleSushi(upc) {
    $.ajax({
        type: "POST",
        url: "../validaBundleSushi.asp",
        dataType: "application/x-www-form-urlencoded",
        data: "Upc=" + upc,
        async: false,
        success: function (msg) {
            $("#fldCom").append(msg);
        }
    })
    return document.getElementById("hdnBundleSushi").value
}
