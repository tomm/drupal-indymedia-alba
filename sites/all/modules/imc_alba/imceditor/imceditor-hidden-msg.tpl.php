<div class="error">
<?php 
	if ($status == IMCEDITOR_NODE_STATUS_DELETED)
		print t("This article is deleted with message: ").$message;
	else
		print t("This article is hidden with message: ").$message;
?>
</div>
