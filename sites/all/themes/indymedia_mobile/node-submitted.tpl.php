<?php
if (isset($node->author)) print 'By <em>'.check_plain($node->author).'</em>, submitted by <em>'.theme('username',$node).'</em> on '.format_date($node->created);
else print 'By <em>'.theme('username',$node).'</em>, submitted on '.format_date($node->created);
