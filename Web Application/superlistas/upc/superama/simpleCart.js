var NextId = 1,
    Custom = "Custom",
    GoogleCheckout = "GoogleCheckout",
    PayPal = "PayPal",
    Email = "Email",
    AustralianDollar = AUD = "AUD",
    CanadianDollar = CAD = "CAD",
    CzechKoruna = CZK = "CZK",
    DanishKrone = DKK = "DKK",
    Euro = EUR = "EUR",
    HongKongDollar = HKD = "HKD",
    HungarianForint = HUF = "HUF",
    IsraeliNewSheqel = ILS = "ILS",
    JapaneseYen = JPY = "JPY",
    MexicanPeso = MXN = "MXN",
    NorwegianKrone = NOK = "NOK",
    NewZealandDollar = NZD = "NZD",
    PolishZloty = PLN = "PLN",
    PoundSterling = GBP = "GBP",
    SingaporeDollar = SGD = "SGD",
    SwedishKrona = SEK = "SEK",
    SwissFranc = CHF = "CHF",
    USDollar = USD = "USD";

function Cart() {
    this.Version = "2.0";
    this.Shelf = new Shelf();
    this.items = {};
    this.isLoaded = false;
    this.pageIsReady = false;
    this.quantity = 0;
    this.total = 0;
    this.numItems = 0;
    this.taxRate = 0;
    this.taxCost = 0;
    this.shippingFlatRate = 0;
    this.shippingTotalRate = 0;
    this.shippingQuantityRate = 0;
    this.shippingRate = 0;
    this.shippingCost = 0;
    this.currency = USD;
    this.checkoutTo = PayPal;
    this.email = "";
    this.merchantId = "";
    this.cartHeaders = ["Name", "Price", "Quantity", "Total", "NumItems"];
    this.add = function () {
        var o = 0;
        var k = 0;
        if ($("#TotalCart").val() != "") {
            var p = $("#TotalCart").val();
            var l = p.split("|");
            k = parseInt(l[0]);
            o = parseFloat(l[1])
        }
        if (k >= 160) {
            alert("Carrito lleno");
            return
        }
        if (arguments[4] == "") {
            return
        }
        if (!this.pageIsReady) {
            this.initializeView();
            this.update()
        }
        if (!this.isLoaded) {
            this.load();
            this.update()
        }
        var n = new CartItem();
        n.price = arguments[1];
        n.upc = arguments[2];
        n.mesu = arguments[5].substring(0, 1).toLowerCase();
        if (n.mesu == "c") {
            n.quantity = parseInt(arguments[4], 10)
        } else {
            n.quantity = Math.round(arguments[4] * 100) / 100
        }
        n.name = arguments[0].replace(/ /g, "&6");
        n.name = n.name.replace(/á/g, "a");
        n.name = n.name.replace(/é/g, "e");
        n.name = n.name.replace(/í/g, "i");
        n.name = n.name.replace(/ó/g, "o");
        n.name = n.name.replace(/ú/g, "u");
        n.name = n.name.replace(/Á/g, "A");
        n.name = n.name.replace(/É/g, "E");
        n.name = n.name.replace(/Í/g, "I");
        n.name = n.name.replace(/Ó/g, "O");
        n.name = n.name.replace(/Ú/g, "U");
        n.name = n.name.replace(/Ñ/g, "N");
        n.name = n.name.replace(/ñ/g, "ñ");
        if (!arguments || arguments.length === 0) {
            error("No values passed for item.");
            return
        }
        var m = arguments;
        n.checkQuantityAndPrice();
        if (arguments[6] != null) {
            n.comment = arguments[6];
            n.comment = arguments[6].replace(/ /g, "&6")
        } else {
            n.comment = ""
        }
        var h = 0;
        if (this.hasItem(n)) {
            h = 0;
            var g = this.hasItem(n);
            this.items[g].quantity = parseFloat(this.items[g].quantity) + parseFloat(n.quantity);
            this.items[g].comment = this.items[g].comment + " " + n.comment
        } else {
            h = 1;
            this.items[n.id] = n
        }
        k = k + h;
        o = o + n.price * n.quantity;
        o = Math.round(o * 100) / 100;
        var j = k + "|" + o;
        if (o < 0) {
            o = 0
        }
        $("#TotalCart").val(k + "|" + o);
        createCookie("superamaTotal", k + "|" + o, 365);
        this.update()
    };
    this.remove = function (g) {
        var n = {};
        for (var o in this.items) {
            var l = this.items[o];
            if (o != g) {
                n[o] = this.items[o]
            }
        }
        var m = 0;
        var j = 0;
        if ($("#TotalCart").val() != "") {
            var p = $("#TotalCart").val();
            var k = p.split("|");
            j = parseInt(k[0]);
            m = parseFloat(k[1])
        }
        j = j - 1;
        m = m - parseFloat(this.items[g].price) * parseFloat(this.items[g].quantity);
        m = Math.round(m * 100) / 100;
        var h = j + "|" + m;
        if (m < 0) {
            m = 0
        }
        $("#TotalCart").val(j + "|" + m);
        createCookie("superamaTotal", j + "|" + m, 365);
        this.items = n
    };
    this.removeUPC = function (h) {
        for (var g in this.items) {
            if (this.items[g]["upc"] == h) {
                simpleCart.items[g].remove()
            }
        }
    };
    this.setQuantityUPC = function (j, h) {
        for (var g in this.items) {
            if (this.items[g]["upc"] == j) {
                simpleCart.items[g].set("quantity", h)
            }
        }
    };
    this.setQuantityUPCMesurable = function (l, k, m) {
        for (var j in this.items) {
            if (this.items[j]["upc"] == l) {
                var h = m.substring(0, 1).toLowerCase();
                var g = k;
                if (h == "c") {
                    g = parseInt(Math.ceil(k), 10)
                }
                simpleCart.items[j].set("quantity", g.toString())
            }
        }
    };
    this.setCommentUPC = function (j, h) {
        var g = new CartItem();
        g.upc = j;
        g.comment = h;
        var k = this.hasItem(g);
        this.items[k].comments = h.replace(/ /g, "&6");
        simpleCart.update()
    };
    this.empty = function () {
        eraseCookie("superamaTotal");
        eraseCookie("TotalCart");
        for (i = 0; i < 9; i++) {
            eraseCookie("simpleCart" + i);
            var g = i + 1;
            eraseCookie("cartString" + g)
        }
        simpleCart.items = {};
        simpleCart.update()
    };
    this.checkout = function () {
        if (simpleCart.quantity === 0) {
            error("Cart is empty");
            return
        }
        switch (simpleCart.checkoutTo) {
        case PayPal:
            simpleCart.paypalCheckout();
            break;
        case GoogleCheckout:
            simpleCart.googleCheckout();
            break;
        case Email:
            simpleCart.emailCheckout();
            break;
        default:
            simpleCart.customCheckout();
            break
        }
    };
    this.paypalCheckout = function () {
        var k = "scrollbars,location,resizable,status",
            j = "https://www.paypal.com/cgi-bin/webscr?cmd=_cart&upload=1&business=" + this.email + "&currency_code=" + this.currency,
            h = 1,
            g = "";
        if (this.taxRate) {
            j = j + "&tax_cart=" + this.currencyStringForPaypalCheckout(this.taxCost)
        }
        for (var o in this.items) {
            var l = this.items[o];
            var n = "";
            for (var m in l) {
                if (typeof (l[m]) != "function" && m != "id" && m != "price" && m != "quantity" && m != "name") {
                    n = n + "&" + m + "=" + l[m]
                }
            }
            n = n.substring(1);
            g = g + "&item_name_" + h + "=" + l.name + "&item_number_" + h + "=" + h + "&quantity_" + h + "=" + l.quantity + "&amount_" + h + "=" + this.currencyStringForPaypalCheckout(l.price) + "&on0_" + h + "=Options&os0_" + h + "=" + n;
            h++
        }
        if (this.shipping() != 0) {
            g = g + "&item_name_" + h + "=Shipping&item_number_" + h + "=" + h + "&quantity_" + h + "=1&amount_" + h + "=" + this.currencyStringForPaypalCheckout(this.shippingCost)
        }
        j = j + g;
        window.open(j, "paypal", k)
    };
    this.googleCheckout = function () {
        if (this.currency != USD && this.currency != GBP) {
            error("Google Checkout only allows the USD and GBP for currency.");
            return
        } else {
            if (this.merchantId === "" || this.merchantId === null || !this.merchantId) {
                error("No merchant Id for google checkout supplied.");
                return
            }
        }
        var k = document.createElement("form"),
            g = 1;
        k.style.display = "none";
        k.method = "POST";
        k.action = "https://checkout.google.com/api/checkout/v2/checkoutForm/Merchant/" + this.merchantId;
        k.acceptCharset = "utf-8";
        for (var m in this.items) {
            var j = this.items[m];
            k.appendChild(this.createHiddenElement("item_name_" + g, j.name));
            k.appendChild(this.createHiddenElement("item_quantity_" + g, j.quantity));
            k.appendChild(this.createHiddenElement("item_price_" + g, j.price));
            k.appendChild(this.createHiddenElement("item_currency_" + g, this.currency));
            k.appendChild(this.createHiddenElement("item_tax_rate_" + g, this.taxRate));
            k.appendChild(this.createHiddenElement("_charset_", ""));
            var h = "";
            for (var l in j) {
                if (typeof (j[l]) != "function" && l != "id" && l != "quantity" && l != "price") {
                    h = h + ", " + l + ": " + j[l]
                }
            }
            h = h.substring(1);
            k.appendChild(this.createHiddenElement("item_description_" + g, h))
        }
        document.body.appendChild(k);
        k.submit();
        document.body.removeChild(k)
    };
    this.emailCheckout = function () {
        return
    };
    this.customCheckout = function () {
        return
    };
    var a = 80;
    var b = 1;
    var d = 1;
    var f = true;
    var c = true;
    var e;
    this.load = function () {
        this.items = {};
        this.total = 0;
        this.quantity = 0;
        this.numItems = 0;
        e = b;
        this.updateTotals();
        var k = b + d;
        if ($("#cartString" + k).val()) {
            $("#btnSiguiente").show()
        } else {
            $("#btnSiguiente").hide()
        }
        var m = b - d;
        if ($("#cartString" + m).val()) {
            $("#btnAnterior").show()
        } else {
            $("#btnAnterior").hide()
        }
        while ($("#cartString" + e).val()) {
            var l = unescape($("#cartString" + e).val()).split("~~");
            for (var g = 0, h = l.length; g < h; g++) {
                var n = l[g].split("||");
                var j = new CartItem();
                if (j.parseValuesFromArray(n)) {
                    j.checkQuantityAndPrice();
                    this.items[j.id] = j
                }
            }
            e++
        }
        this.isLoaded = true
    };
    this.save = function () {
        e = b;
        var j = "";
        var h = 0;
        var g = "simpleCart" + e;
        var l = 4000;
        for (var k in this.items) {
            h = h + 1;
            j = j + "~~" + this.items[k].print();
            if (j.length >= l) {
                createCookie("simpleCart" + (e - 1), j.substring(2), 365);
                $("#cartString" + e).val(j.substring(2));
                e++;
                j = ""
            }
        }
        createCookie("simpleCart" + (e - 1), j.substring(2), 365);
        $("#cartString" + e).val(j.substring(2))
    };
    this.initializeView = function () {
        this.numItemsOutlets = getElementsByClassName("simpleCart_numItems");
        this.totalOutlets = getElementsByClassName("simpleCart_total");
        this.quantityOutlets = getElementsByClassName("simpleCart_quantity");
        this.cartDivs = getElementsByClassName("simpleCart_items");
        this.taxCostOutlets = getElementsByClassName("simpleCart_taxCost");
        this.taxRateOutlets = getElementsByClassName("simpleCart_taxRate");
        this.shippingCostOutlets = getElementsByClassName("simpleCart_shippingCost");
        this.finalTotalOutlets = getElementsByClassName("simpleCart_finalTotal");
        this.addEventToArray(getElementsByClassName("simpleCart_checkout"), simpleCart.checkout, "click");
        this.addEventToArray(getElementsByClassName("simpleCart_empty"), simpleCart.empty, "click");
        this.Shelf.readPage();
        this.pageIsReady = true
    };
    this.updateView = function () {
        if (this.numItems < 1) {
            desactivarControles()
        } else {
            activarControles()
        }
        this.updateViewTotals();
        if (this.cartDivs && this.cartDivs.length > 0) {
            this.updateCartView()
        }
    };
    this.updateViewTotals = function () {
        var m = [
            ["quantity", "none"],
            ["total", "currency"],
            ["shippingCost", "currency"],
            ["taxCost", "currency"],
            ["taxRate", "percentage"],
            ["numItems", "none"],
            ["finalTotal", "currency"]
        ];
        for (var g = 0, h = m.length; g < h; g++) {
            var l = m[g][0] + "Outlets",
                k;
            for (var j in this[l]) {
                switch (m[g][1]) {
                case "none":
                    k = "" + this[m[g][0]];
                    break;
                case "currency":
                    k = this.valueToCurrencyString(this[m[g][0]]);
                    break;
                case "percentage":
                    k = this.valueToPercentageString(this[m[g][0]]);
                    break;
                default:
                    k = "" + this[m[g][0]];
                    break
                }
                this[l][j].innerHTML = "" + k
            }
        }
    };
    this.updateCartView = function () {
        var o = [],
            u, z, w, p, n, l, h, s, m, t;
        z = document.createElement("div");
        for (n in this.cartHeaders) {
            l = document.createElement("div");
            t = this.cartHeaders[n].split("_");
            l.innerHTML = t[0];
            l.className = "item" + t[0];
            for (u = 1, xlen = t.length; u < xlen; u++) {
                if (t[u].toLowerCase() == "noheader") {
                    l.style.display = "none"
                }
            }
            z.appendChild(l)
        }
        z.className = "cartHeaders";
        o[0] = z;
        u = 1;
        for (p in this.items) {
            z = document.createElement("div");
            w = this.items[p];
            for (n in this.cartHeaders) {
                l = document.createElement("div");
                h = this.cartHeaders[n].split("_");
                switch (h[0].toLowerCase()) {
                case "total":
                    s = this.valueToCurrencyString(parseFloat(w.price) * parseFloat(w.quantity));
                    break;
                case "increment":
                    s = this.valueToLink("+", "javascript:;", "onclick=\"simpleCart.items['" + w.id + "'].increment();_gaq.push(['_trackEvent', 'Carrito', 'Articulos', 'Incrementar']);\"");
                    break;
                case "decrement":
                    s = this.valueToLink("-", "javascript:;", "onclick=\"simpleCart.items['" + w.id + "'].decrement();_gaq.push(['_trackEvent', 'Carrito', 'Articulos', 'Decrementar']);\"");
                    break;
                case "remove":
                    s = this.valueToLink("x", "javascript:;", "onclick=\"simpleCart.items['" + w.id + "'].remove();_gaq.push(['_trackEvent', 'Carrito', 'Articulos', 'Eliminar']);\"");
                    break;
                case "price":
                    s = this.valueToCurrencyString(w[h[0].toLowerCase()] ? w[h[0].toLowerCase()] : " ");
                    break;
                default:
                    var q = w[h[0].toLowerCase()] ? w[h[0].toLowerCase()] : " ";
                    q = q + "";
                    q = q.replace(/&6/g, " ");
                    s = q;
                    break
                }
                for (var r = 1, A = h.length; r < A; r++) {
                    m = h[r].toLowerCase();
                    switch (m) {
                    case "image":
                    case "img":
                        s = this.valueToImageString(s);
                        break;
                    case "input":
                        s = this.valueToTextInput(s, "onchange=\"checaPesable(this,'" + w.mesu + "');simpleCart.items['" + w.id + "'].set('" + s + "' , this.value);_gaq.push(['_trackEvent', 'Carrito', 'Articulos', 'Cambiar cantidad en campo']);\" maxlength='4' onkeypress=checkKey('" + w.id + "',this.value,'" + s + "','" + w.mesu + "')");
                        break;
                    case "div":
                    case "span":
                    case "h1":
                    case "h2":
                    case "h3":
                    case "h4":
                    case "p":
                        s = this.valueToElement(m, s, "");
                        break;
                    case "noheader":
                        break;
                    default:
                        error("unkown header option: " + m);
                        break
                    }
                }
                l.innerHTML = s;
                l.className = "item" + h[0];
                z.appendChild(l)
            }
            z.className = "itemContainer";
            z.style.zIndex = "-1";
            o[u] = z;
            u++
        }
        for (p in this.cartDivs) {
            var g = this.cartDivs[p];
            while (g.childNodes[0]) {
                g.removeChild(g.childNodes[0])
            }
            miRow = document.createElement("div");
            miRow.className = "carrito";
            miRow.id = "carrito";
            for (var k = 0, v = o.length; k < v; k++) {
                miRow.appendChild(o[k])
            }
            g.appendChild(miRow)
        }
    };
    this.addEventToArray = function (l, k, j) {
        for (var h in l) {
            var g = l[h];
            if (g.addEventListener) {
                g.addEventListener(j, k, false)
            } else {
                if (g.attachEvent) {
                    g.attachEvent("on" + j, k)
                }
            }
        }
    };
    this.createHiddenElement = function (g, j) {
        var h = document.createElement("input");
        h.type = "hidden";
        h.name = g;
        h.value = j;
        return h
    };
    this.currencySymbol = function () {
        switch (this.currency) {
        case JPY:
            return "&yen;";
        case EUR:
            return "&euro;";
        case GBP:
            return "&pound;";
        case USD:
        case CAD:
        case AUD:
        case NZD:
        case HKD:
        case SGD:
            return "&#36;";
        default:
            return ""
        }
    };
    this.currencyStringForPaypalCheckout = function (g) {
        if (this.currencySymbol == "&#36;") {
            return "$" + parseFloat(g).toFixed(2)
        } else {
            return "" + parseFloat(g).toFixed(2)
        }
    };
    this.valueToCurrencyString = function (g) {
        return parseFloat(g).toCurrency(this.currencySymbol())
    };
    this.valueToPercentageString = function (g) {
        return parseFloat(100 * g).toFixed(0) + "%"
    };
    this.valueToImageString = function (g) {
        if (g.match(/<\s*img.*src\=/)) {
            return g
        } else {
            return '<img src="' + g + '" />'
        }
    };
    this.valueToTextInput = function (h, g) {
        return '<input type="text" value="' + h + '" ' + g + " />"
    };
    this.valueToLink = function (j, h, g) {
        return '<a href="' + h + '" ' + g + " >" + j + "</a>"
    };
    this.valueToElement = function (h, j, g) {
        return "<" + h + " " + g + " > " + j + "</" + h + ">"
    };
    this.hasItem = function (h) {
        for (var l in this.items) {
            var g = this.items[l];
            var j = true;
            for (var k in h) {
                if (typeof (h[k]) != "function" && k == "upc") {
                    if (h[k] != g[k]) {
                        j = false
                    }
                }
            }
            if (j) {
                return l
            }
        }
        return false
    };
    this.update = function () {
        if (!simpleCart.isLoaded) {
            simpleCart.load()
        }
        if (!simpleCart.pageIsReady) {
            simpleCart.initializeView()
        }
        this.updateTotals();
        this.updateView();
        this.save()
    };
    this.updateTotals = function () {
        this.numItems = 0;
        this.total = 0;
        this.quantity = 0;
        for (var m in this.items) {
            var j = this.items[m];
            if (j.quantity <= 0) {
                j.remove()
            } else {
                if (j.quantity !== null && j.quantity != "undefined") {
                    this.quantity = parseFloat(this.quantity) + parseFloat(j.quantity)
                }
            }
            if (j.price) {
                var l = 0;
                var h = 0;
                if ($("#TotalCart").val() != "") {
                    var g = $("#TotalCart").val();
                    var k = g.split("|");
                    l = parseInt(k[0]);
                    h = parseFloat(k[1])
                }
                this.numItems = l;
                this.total = h
            }
        }
        this.shippingCost = this.shipping();
        this.taxCost = parseFloat(this.total) * this.taxRate;
        this.finalTotal = this.shippingCost + this.taxCost + this.total
    };
    this.shipping = function () {
        if (parseFloat(this.quantity) === 0) {
            return 0
        }
        var h = parseFloat(this.shippingFlatRate) + parseFloat(this.shippingTotalRate) * parseFloat(this.total) + parseFloat(this.shippingQuantityRate) * parseFloat(this.quantity),
            g, j;
        for (j in this.items) {
            g = this.items[j];
            if (g.shipping) {
                if (typeof g.shipping == "function") {
                    h += parseFloat(g.shipping())
                } else {
                    h += parseFloat(g.shipping)
                }
            }
        }
        return h
    };
    this.initialize = function () {
        $("#btnSiguiente").click(function () {
            b = b + d;
            simpleCart.load();
            simpleCart.update()
        });
        $("#btnAnterior").click(function () {
            b = b - d;
            simpleCart.load();
            simpleCart.update()
        });
        simpleCart.initializeView();
        simpleCart.load();
        simpleCart.update()
    }
}
function checaPesable(c, b) {
    if (b == "c") {
        var a = parseInt(c.value, 10);
        if (isNaN(a)) {
            c.value = "1"
        } else {
            c.value = a
        }
    }
}
function checkKey(d, c, a, e) {
    if (window.event.keyCode == 13) {
        if (e == "c") {
            var b = parseInt(c, 10);
            if (isNaN(b)) {
                c = "1"
            } else {
                c = b
            }
        }
        simpleCart.items[d].set(a, c)
    }
}
function CartItem(a) {
    this.id = a
}
function CartItem() {
    this.id = "c" + NextId++
}
CartItem.prototype.set = function (g, f) {
    var c = 0;
    var e = 0;
    if ($("#TotalCart").val() != "") {
        var b = $("#TotalCart").val();
        var d = b.split("|");
        e = parseInt(d[0]);
        c = parseFloat(d[1])
    }
    if (f <= 0) {
        doIt = confirm("\u00BFDeseas eliminar el art\u00edculo del carrito?");
        if (doIt) {} else {
            f = this.quantity
        }
    }
    g = g.toLowerCase();
    c = c - this.price * this.quantity;
    this.quantity = parseFloat(this.quantity) + 1;
    if (typeof (this[g]) != "function" && g != "id") {
        if (g == "quantity") {
            f = f.replace(/[^(\d|\.)]*/gi, "");
            f = f.replace(/,*/gi, "");
            f = parseFloat(f)
        } else {
            if (g == "price") {
                f = f.replace(/[^(\d|\.)]*/gi, "");
                f = f.replace(/,*/gi, "");
                f = parseFloat(f)
            }
        }
        if (typeof (f) == "number" && isNaN(f)) {
            error("Improperly formatted input.")
        } else {}
    } else {
        error("Cannot change " + g + ", this is a reserved field.")
    }
    this.quantity = f;
    c = c + this.price * this.quantity;
    c = Math.round(c * 100) / 100;
    var a = e + "|" + c;
    if (c < 0) {
        c = 0
    }
    $("#TotalCart").val(e + "|" + c);
    createCookie("superamaTotal", e + "|" + c, 365);
    simpleCart.update()
};
CartItem.prototype.increment = function () {
    this.quantity = parseFloat(this.quantity) + 1;
    var c = 0;
    var e = 0;
    if ($("#TotalCart").val() != "") {
        var b = $("#TotalCart").val();
        var d = b.split("|");
        e = parseInt(d[0]);
        c = parseFloat(d[1])
    }
    c = c + parseFloat(this.price);
    c = Math.round(c * 100) / 100;
    var a = e + "|" + c;
    if (c < 0) {
        c = 0
    }
    $("#TotalCart").val(e + "|" + c);
    createCookie("superamaTotal", e + "|" + c, 365);
    simpleCart.update()
};
CartItem.prototype.setQuantity = function (f) {
    this.quantity = parseFloat(f);
    var c = 0;
    var e = 0;
    if ($("#TotalCart").val() != "") {
        var b = $("#TotalCart").val();
        var d = b.split("|");
        e = parseInt(d[0]);
        c = parseFloat(d[1])
    }
    c = c - parseFloat(this.price) * parseFloat(this.quantity);
    c = Math.round(c * 100) / 100;
    var a = e + "|" + c;
    if (c < 0) {
        c = 0
    }
    createCookie("superamaTotal", e + "|" + c, 365);
    $("#TotalCart").val(e + "|" + c);
    simpleCart.update()
};
CartItem.prototype.decrement = function () {
    var c = 0;
    var e = 0;
    if ($("#TotalCart").val() != "") {
        var b = $("#TotalCart").val();
        var d = b.split("|");
        e = parseInt(d[0]);
        c = parseFloat(d[1])
    }
    if (parseFloat(this.quantity) < 2) {
        doIt = confirm("\u00BFRealmente deseas borrar el art\u00edculo?");
        if (doIt) {} else {
            return
        }
    }
    if (parseFloat(this.quantity) < 2) {
        e = e - 1;
        c = c - parseFloat(this.price);
        this.remove()
    } else {
        this.quantity = parseFloat(this.quantity) - 1;
        c = c - parseFloat(this.price)
    }
    c = Math.round(c * 100) / 100;
    var a = e + "|" + c;
    if (c < 0) {
        c = 0
    }
    createCookie("superamaTotal", e + "|" + c, 365);
    $("#TotalCart").val(e + "|" + c);
    simpleCart.update()
};
CartItem.prototype.print = function () {
    var a = "";
    for (var b in this) {
        if (typeof (this[b]) != "function") {
            a += this[b] + "||"
        }
    }
    return a.substring(0, a.length - 2)
};
CartItem.prototype.checkQuantityAndPrice = function () {
    if (!this.price || this.quantity == null || this.quantity == "undefined") {
        this.quantity = 1
    } else {
        this.quantity = ("" + this.quantity).replace(/,*/gi, "");
        this.quantity = parseFloat(("" + this.quantity).replace(/[^(\d|\.)]*/gi, ""));
        if (isNaN(this.quantity)) {
            error("Quantity is not a number.");
            this.quantity = 1
        }
    }
    if (!this.price || this.price == null || this.price == "undefined") {
        this.price = 0;
        error("No price for item or price not properly formatted.")
    } else {
        this.price = ("" + this.price).replace(/,*/gi, "");
        this.price = parseFloat(("" + this.price).replace(/[^(\d|\.)]*/gi, ""));
        if (isNaN(this.price)) {
            error("Price is not a number.");
            this.price = 0
        }
    }
};
CartItem.prototype.parseValuesFromArray = function (c) {
    if (c && c.length && c.length > 0) {
        for (var a = 0, b = c.length; a < b; a++) {
            c[a].replace(/||/, "| |");
            c[a].replace(/\+\+/, "+ +");
            this["price"] = unescape(c[1]);
            this["upc"] = unescape(c[2]);
            this["mesu"] = unescape(c[3]);
            this["quantity"] = unescape(c[4]);
            this["name"] = unescape(c[5]);
            this["comments"] = unescape(c[6])
        }
        return true
    } else {
        return false
    }
};
CartItem.prototype.remove = function () {
    simpleCart.remove(this.id);
    simpleCart.update()
};

function Shelf() {
    this.items = {}
}
Shelf.prototype.readPage = function () {
    this.items = {};
    var a = getElementsByClassName("simpleCart_shelfItem");
    for (var c in a) {
        var b = new ShelfItem();
        this.checkChildren(a[c], b);
        this.items[b.id] = b
    }
};
Shelf.prototype.checkChildren = function (d, b) {
    for (var a = 0; d.childNodes[a]; a++) {
        var c = d.childNodes[a];
        if (c.className && c.className.match(/item_/)) {
            var e = c.className.split("_");
            if (e[1] == "add" || e[1] == "Add") {
                var g = [];
                g.push(c);
                var f = simpleCart.Shelf.addToCart(b.id);
                simpleCart.addEventToArray(g, f, "click");
                c.id = b.id
            } else {
                b[e[1]] = c
            }
        }
        if (c.childNodes[0]) {
            this.checkChildren(c, b)
        }
    }
};
Shelf.prototype.empty = function () {
    this.items = {}
};
Shelf.prototype.addToCart = function (a) {
    return function () {
        if (simpleCart.Shelf.items[a]) {
            simpleCart.Shelf.items[a].addToCart()
        } else {
            error("Shelf item with id of " + a + " does not exist.")
        }
    }
};

function ShelfItem() {
    this.id = "s" + NextId++
}
ShelfItem.prototype.remove = function () {
    simpleCart.Shelf.items[this.id] = null
};
ShelfItem.prototype.addToCart = function () {
    var c = [],
        a;
    for (var b in this) {
        if (typeof (this[b]) != "function" && b != "id") {
            a = "";
            switch (b) {
            case "price":
                if (this[b].value) {
                    a = this[b].value
                } else {
                    if (this[b].innerHTML) {
                        a = this[b].innerHTML
                    }
                }
                a = a.replace(/[^(\d|\.)]*/gi, "");
                a = a.replace(/,*/, "");
                break;
            case "image":
                a = this[b].src;
                break;
            default:
                if (this[b].value) {
                    a = this[b].value
                } else {
                    if (this[b].innerHTML) {
                        a = this[b].innerHTML
                    } else {
                        if (this[b].src) {
                            a = this[b].src
                        } else {
                            a = this[b]
                        }
                    }
                }
                break
            }
            c.push(b + "=" + a)
        }
    }
    simpleCart.add(c)
};

function createCookie(c, d, e) {
    if (e) {
        var b = new Date();
        b.setTime(b.getTime() + (e * 24 * 60 * 60 * 1000));
        var a = "; expires=" + b.toGMTString()
    } else {
        var a = ""
    }
    document.cookie = c + "=" + d + a + "; path=/"
}
function readCookie(b) {
    var e = b + "=";
    var a = document.cookie.split(";");
    for (var d = 0; d < a.length; d++) {
        var f = a[d];
        while (f.charAt(0) == " ") {
            f = f.substring(1, f.length)
        }
        if (f.indexOf(e) === 0) {
            return f.substring(e.length, f.length)
        }
    }
    return null
}
function eraseCookie(a) {
    createCookie(a, "", -1);
    $("#" + a).val("")
}
activarControles = function () {
    var a = $("#comprar");
    a.show()
};
desactivarControles = function () {
    var a = $("#comprar");
    a.hide()
};
var getElementsByClassName = function (b, a, c) {
        if (document.getElementsByClassName) {
            getElementsByClassName = function (j, m, h) {
                h = h || document;
                var d = h.getElementsByClassName(j),
                    l = (m) ? new RegExp("\\b" + m + "\\b", "i") : null,
                    e = [],
                    g;
                for (var f = 0, k = d.length; f < k; f += 1) {
                    g = d[f];
                    if (!l || l.test(g.nodeName)) {
                        e.push(g)
                    }
                }
                return e
            }
        } else {
            if (document.evaluate) {
                getElementsByClassName = function (p, s, o) {
                    s = s || "*";
                    o = o || document;
                    var g = p.split(" "),
                        q = "",
                        m = "http://www.w3.org/1999/xhtml",
                        r = (document.documentElement.namespaceURI === m) ? m : null,
                        h = [],
                        d, f;
                    for (var k = 0, l = g.length; k < l; k += 1) {
                        q += "[contains(concat(' ', @class, ' '), ' " + g[k] + " ')]"
                    }
                    try {
                        d = document.evaluate(".//" + s + q, o, r, 0, null)
                    } catch (n) {
                        d = document.evaluate(".//" + s + q, o, null, 0, null)
                    }
                    while ((f = d.iterateNext())) {
                        h.push(f)
                    }
                    return h
                }
            } else {
                getElementsByClassName = function (s, v, r) {
                    v = v || "*";
                    r = r || document;
                    var h = s.split(" "),
                        u = [],
                        d = (v === "*" && r.all) ? r.all : r.getElementsByTagName(v),
                        q, n = [],
                        p;
                    for (var j = 0, e = h.length; j < e; j += 1) {
                        u.push(new RegExp("(^|\\s)" + h[j] + "(\\s|$)"))
                    }
                    for (var g = 0, t = d.length; g < t; g += 1) {
                        q = d[g];
                        p = false;
                        for (var f = 0, o = u.length; f < o; f += 1) {
                            p = u[f].test(q.className);
                            if (!p) {
                                break
                            }
                        }
                        if (p) {
                            n.push(q)
                        }
                    }
                    return n
                }
            }
        }
        return getElementsByClassName(b, a, c)
    };
String.prototype.reverse = function () {
    return this.split("").reverse().join("")
};
Number.prototype.withCommas = function () {
    var a = 6,
        b = parseFloat(this).toFixed(2).toString().reverse();
    while (a < b.length) {
        b = b.substring(0, a) + "," + b.substring(a);
        a += 4
    }
    return b.reverse()
};
Number.prototype.toCurrency = function () {
    return (arguments[0] ? arguments[0] : "$") + this.withCommas()
};

function error(b) {
    try {
        console.log(b)
    } catch (a) {}
}
var simpleCart = new Cart();
window.onload = simpleCart.initialize;
