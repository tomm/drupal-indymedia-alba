-- I use this when testing the module, to allow repeated runs.
-- It wipes articles, imported users and some other stuff.
-- XXX Don't use this on a live site.
delete from node;
delete from node_comment_statistics;
delete from node_revisions;
delete from users where uid > 900;
delete from dada_articles;
delete from dada_features;
delete from comments;
delete from cache_menu;
delete from cache;
delete from cache_filter;
delete from cache_form;
delete from cache_page;
delete from cache_update;
delete from watchdog;
delete from batch;
delete from vocabulary;
delete from term_data;
delete from term_node;
delete from term_hierarchy;
delete from term_relation;
delete from term_synonym;
delete from imc_node_moderation;
delete from imc_feature_proposal;
delete from imc_feature_vote;
delete from search_index;
delete from search_dataset;
delete from users_roles;
