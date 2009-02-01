
function on_state_change() {
	if (g_nodeIsHidden) {
		$("div#editor_moderate_node_visible").hide();
		$("div#editor_moderate_node_hidden").show();
		$("span#moderate_hidden_reason").html(g_nodeHiddenReason);
	} else {
		$("div#editor_moderate_node_visible").show();
		$("div#editor_moderate_node_hidden").hide();
	}
	if (g_canPropose) $("div#editor_propose_feature").show();
	else $("div#editor_propose_feature").hide();

	if (g_canUnpropose) $("div#editor_unpropose_feature").show();
	else $("div#editor_unpropose_feature").hide();

	if (g_canVote) $("div#editor_vote_feature").show();
	else $("div#editor_vote_feature").hide();
			
	if (g_canRetractVote) $("div#editor_retract_vote").show();
	else $("div#editor_retract_vote").hide();
}

function update_msgs(data) {
	if (data['mod_msgs']) $("div#editor_mod_msgs").html(data['mod_msgs']);
	if (data['feature_msg']) $("div#editor_feature_msg").html(data['feature_msg']);
}

$(document).ready(function() {
	$("div#editor_moderate div.editor_heading").click(function() {
		$("div#editor_moderate_ctrls").toggle("slow");

	});
	on_state_change();
	$("div#editor_moderate_ctrls").hide();

	$("button#editor_moderate_hide").click(function() {
		var reason = $("input#moderate_hide_reason").attr("value");
		if (!reason) {
			alert("You must specify why you are hiding this article");
		} else {
			$.getJSON(editor_cpan['ajax_url']+"&op=hide&nid="+g_nodeNid+"&reason="+escape(reason), function(data) {
				$("div#hidden-msg-"+g_nodeNid).html(data['hidden_msg']);
				g_nodeHiddenReason = reason;
				g_nodeIsHidden = true;
				on_state_change();
			});
		}
	});

	$("button#editor_moderate_delete").click(function() {
		var reason = $("input#moderate_hide_reason").attr("value");
		if (window.confirm(Drupal.t("Are you sure you want to delete this article?\n"+
			"Articles can be un-deleted by editors for a limited period"))) {
			$.getJSON(editor_cpan['ajax_url']+"&op=delete&nid="+g_nodeNid+"&reason="+escape(reason), function(data) {
				$("div#hidden-msg-"+g_nodeNid).html(data['hidden_msg']);
				g_nodeHiddenReason = reason;
				g_nodeIsHidden = true;
				on_state_change();
			});
		}
	});
	
	$("button#editor_moderate_unhide").click(function() {
		$.getJSON(editor_cpan['ajax_url']+"&op=unhide&nid="+g_nodeNid, function(data) {
			//alert(data["status"]);
			$("div#hidden-msg-"+g_nodeNid).html("");
			g_nodeIsHidden = false;
			on_state_change();
		});
	});

	$("#make_feature_button").click(function() {
		var comment = $("input#make_feature_comment").attr("value");
		if (!comment) comment = "";
		$.getJSON(editor_cpan['ajax_url']+"&op=makefeature&nid="+g_nodeNid+"&comment="+escape(comment), function (data) {
			if (data['stat']) {
				update_msgs(data);
				g_canPropose = false;
				g_canUnpropose = data['stat'];
				g_canVote = true;
				g_canRetractVote = false;
				on_state_change();
			} else {
				alert(data['msg']);
			}
		});
	});
	
	$("#unpropose_feature_button").click(function() {
		$.getJSON(editor_cpan['ajax_url']+"&op=unmakefeature&nid="+g_nodeNid, function (data) {
			if (data['stat']) {
				update_msgs(data);
				g_canPropose = true;
				g_canUnpropose = false;
				g_canVote = false;
				g_canRetractVote = false;
				on_state_change();
			} else {
				alert(data['msg']);
			}
		});
	});

	$("button#vote_for").click(function() {
		var comment = $("input#vote_feature_comment").attr("value");
		if (!comment) comment = "";
		$.getJSON(editor_cpan['ajax_url']+"&op=votefeature&nid="+g_nodeNid+"&vote=for&comment="+escape(comment), function(data) {
			update_msgs(data);
			g_canVote = data['can_vote'];
			g_canRetractVote = (data['stat'] == 1);
			on_state_change();
		});
	});
	
	$("button#vote_block").click(function() {
		var comment = $("input#vote_feature_comment").attr("value");
		if (!comment) {
			alert("You must state a reason when blocking a feature proposal");
			return;
		}
		$.getJSON(editor_cpan['ajax_url']+"&op=votefeature&nid="+g_nodeNid+"&vote=block&comment="+escape(comment), function(data) {
			update_msgs(data);
			g_canVote = data['can_vote'];
			g_canRetractVote = !g_canVote;
			on_state_change();
		});
	});

	$("button#retract_vote").click(function() {
		$.getJSON(editor_cpan['ajax_url']+"&op=retractvote&nid="+g_nodeNid, function(data) {
			update_msgs(data);
			if (data['stat'] == 2) {
				// retraction of block caused feature
				// promotion
				g_canVote = false;
				g_canRetractVote = false;
				g_canUnpropose = false;
				g_canPropose = false;
			} else {
				g_canVote = data['stat'];
				g_canRetractVote = !g_canVote;
			}
			on_state_change();
		});
	});
});

