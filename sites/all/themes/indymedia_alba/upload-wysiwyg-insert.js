
function update_click_inserts() {
	$("button.insertimg").click(function() {
		//var caption = prompt("If you would like an image caption enter it here, otherwise leave it blank","");
		var imgtag = "<img src=\""+this.id+"\" alt=\"\" />";
		//if (caption) {
		//  imgtag = "<span class=\"imgbox\">"+imgtag+"<div class=\"imgcaption\">"+caption+"</div></span>";
		//}
		Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert(imgtag);
	});
	$("button.insertthumb").click(function() {
		var imgtag = "<img src=\""+this.id+"\" alt=\"\" />";
		Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert(imgtag);
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

