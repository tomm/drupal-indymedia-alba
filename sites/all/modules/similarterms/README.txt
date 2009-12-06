$Id: README.txt,v 1.2 2008/06/19 16:43:46 rmiddle Exp $

Similar By Terms (similarterms.module)
by Jeff Robbins

This Drupal module attempts to provide context for content items by 
displaying a block with links to other similar content. Similarity is 
based on the taxonomy terms assigned to content. Blocks are available 
based on similarity within each of the defined vocabularies for a site 
as well as a block for similarity within all vocabularies.

What does this really mean? How should you use this module?

1) Create a freetagging vocabulary called "Tags" assigned to the content 
types on which you'd like to display the "Similar" block.
2) Enable the block called "Similar entries from the Tags vocabulary".
3) Add keyword tags to the content.

Done!

Now when you go to the page for a tagged content item, a block will show
up displaying other content in descending order of common tags (terms).

We're using this on Lullabot.com, click on any of the articles at 
http://www.lullabot.com/articles in order to see the "Similar" block.