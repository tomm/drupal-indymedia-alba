

$.fn.insertAtCaret = function (myValue) {
        return this.each(function(){
                //IE support
                if (document.selection) {
                        this.focus();
                        sel = document.selection.createRange();
                        sel.text = myValue;
                        this.focus();
                }
                //MOZILLA/NETSCAPE support
                else if (this.selectionStart || this.selectionStart == "0") {
                        var startPos = this.selectionStart;
                        var endPos = this.selectionEnd;
                        var scrollTop = this.scrollTop;
                        this.value = this.value.substring(0, startPos)
                                      + myValue
                              + this.value.substring(endPos,
this.value.length);
                        this.focus();
                        this.selectionStart = startPos + myValue.length;
                        this.selectionEnd = startPos + myValue.length;
                        this.scrollTop = scrollTop;
                } else {
                        this.value += myValue;
                        this.focus();
                }
        });

};

function update_click_inserts() {
	$("button.insertimg").click(function() {
			//var caption = prompt("If you would like an image caption enter it here, otherwise leave it blank","");
			var imgtag = "<img src=\""+this.id+"\" alt=\"\" />";
			//if (caption) {
			//  imgtag = "<span class=\"imgbox\">"+imgtag+"<div class=\"imgcaption\">"+caption+"</div></span>";
			//}
			try {
			$("#edit-body").insertAtCaret(imgtag);
			} catch (error) {}
			try {
			var oEditor = FCKeditorAPI.GetInstance(fckLaunchedJsId[0]) ;
			oEditor.InsertHtml(imgtag);
			} catch (error) {}
			});
	$("button.insertthumb").click(function() {
			var imgtag = "<img src=\""+this.id+"\" alt=\"\" />";
			try {
			$("#edit-body").insertAtCaret(imgtag);
			} catch (error) {}
			try {
			var oEditor = FCKeditorAPI.GetInstance(fckLaunchedJsId[0]) ;
			oEditor.InsertHtml(imgtag);
			} catch (error) {}
			});
}

jQuery.fn["replaceAndUpdateHandlers"] = function(value) {
	this.empty().append(value);
	update_click_inserts();
}

function setup_insert_other_img() {
	$("#insert-other-img").html("poo");
}

$(document).ready(function() {
	update_click_inserts();
	setup_insert_other_img();
});

function open_image_picking_window() {
	var win = window.open("/sites/all/modules/upload/insertotherpopup.html?var=oFCK_1", null, "toolbar=no, location=no, menubar=no, status=no, scrollbars=yes, width=400, height=500");
}

