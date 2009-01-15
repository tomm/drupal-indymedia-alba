<div id="editor_moderate">
	<div class="editor_heading"><?php print t("Editor moderation"); ?></div>
	<noscript>You need Javascript enabled to use this control panel</noscript>
	<div id="editor_moderate_ctrls" style="display: none;">
		<div class="editor_ctrls_subheading">
			<?php print t("Visibility"); ?>
		</div>
		<div class="editor_ctrls_section">
			<div id="editor_moderate_node_visible">
				<button id="editor_moderate_hide"><?php print t("Hide") ?></button>
				<?php print t('Reason:'); ?>
				<input id="moderate_hide_reason" type="text" />
			</div>
			<div id="editor_moderate_node_hidden">
				<?php print t("This post is hidden with note: "); ?>
				<span id="moderate_hidden_reason"></span>
				<div><button id="editor_moderate_unhide"><?php print t("Unhide"); ?></button></div>
			</div>
		</div>
		<div class="editor_ctrls_subheading"><?php print t("Feature promotion"); ?></div>
		<div class="editor_ctrls_section">
			<div id="editor_feature_msg">
				<?php print
				theme('imceditor_cpanel_feature_msg', $node, $prop);
				?>
			</div>
			<div id="editor_propose_feature">
				<button id="make_feature_button"><?php print t("Propose as a feature"); ?></button>
				<?php print t('Comment:'); ?>
				<input id="make_feature_comment" type="text" />
			</div>
			<div id="editor_unpropose_feature">
				<button id="unpropose_feature_button">
					<?php print t("Retract feature proposal"); ?>
				</button>
			</div>
			<div id="editor_vote_feature">
				<button id="vote_for"><?php print t('Vote for feature'); ?></button>
				<button id="vote_block"><?php print t('Block feature'); ?></button>
				
				<?php print t('Comment:'); ?>
				<input id="vote_feature_comment" type="text" />
			</div>
			<div id="editor_retract_vote">
				<button id="retract_vote"><?php print t('Retract your vote'); ?></button>
			</div>
		</div>
	</div>
</div>
