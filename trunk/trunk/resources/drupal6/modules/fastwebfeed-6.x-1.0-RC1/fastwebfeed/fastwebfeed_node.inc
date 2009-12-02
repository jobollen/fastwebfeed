<?php
// $Id$
/**
 * @file
 * fastwebfeed_node.inc
 * 
 * This a ".inc"-file. 
 * 
 * @author Jo Bollen <fastwebfeed@gmail.com>
 * @site http://cupid-project.be/project/fastwebfeed-module
 * @filesource
 */


/**
 * Implementation of the hook_menu_alter()
 */
function fastwebfeed_menu_alter(&$items) {
  $items['rss.xml'] = array(
    'title' => 'RSS feed',
    'page callback' => 'fastwebfeed_handler_node_feed',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );
}

/**
 * Implementation of the hook_fastwebfeed_description()
 * Returns a piece of the Fastwebfeed admin info form
 * 
 * @return $output
 *   string
 *   
 * @see fastwebfeed.module
 */
function fastwebfeed_fastwebfeed_description() {
  global $base_path;
  $module_path = drupal_get_path('module', 'fastwebfeed');
  $module_path_sup = drupal_get_path('module', 'fastwebfeed_sup');
  $module_path_pubsubhubbub = drupal_get_path('module', 'fastwebfeed_pubsubhubbub');
  
  $output = '
  		<ul>
    		<li>fastwebfeed_node.inc
      		<ul>
      			<li>Settings: '. l('Administer > Site configuration > Fastwebfeed > plugged modules - "Settings for the node.module"', 'admin/settings/fastwebfeed/plugged_modules') .'</li>
      			<li>Read '. l('CHANGELOG.txt', $module_path .'/CHANGELOG.txt') .'</li>
      		</ul>
      	</li>
    	</ul>';
  return $output;
}

/**
 * Implementation of the hook_fastwebfeed_plugged_modules_admin()
 * A piece of the Fastwebfeed admin setting form
 * Administer -> Site configuration -> fastwebfeed -> plugged module
 * 
 * @return $form 
 * 
 * @see fastwebfeed.module
 */
function fastwebfeed_fastwebfeed_plugged_modules_admin() {
  global $base_url, $base_path;
  $fastwebfeed_module_path = drupal_get_path('module', 'fastwebfeed');
  
  $form['fieldset_fastwebfeed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for the node.module'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['fieldset_fastwebfeed']['fieldset_when_to_ping'] = array(
    '#type' => 'fieldset',
    '#title' => t('When to ping (or notify)?'),
    '#description' => t('The question is, when to ping a service (or notify a hub) with an updated "default" feed?'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fieldset_fastwebfeed']['fieldset_when_to_ping']['fastwebfeed_check_ping_on_node_submission_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('On "node" submission.'),
    '#description' => t('If checked, the hubs/services will be notified on "node" submission with a update notification refered to the updated "default" feed.'),
    '#default_value' => variable_get('fastwebfeed_check_ping_on_node_submission_admin', ''),
  );
  
  $form['fieldset_fastwebfeed']['fieldset_what_to_ping'] = array(
    '#type' => 'fieldset',
    '#title' => t('What to ping (or notify)?'),
    '#description' => t('The question is, which "default" feed must be pinged to a service? (or which feed must be notified to a hub?)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fieldset_fastwebfeed']['fieldset_what_to_ping']['fieldset_default_rss_feed'] = array(
    '#type' => 'fieldset',
    '#title' => t('rss.xml feed'),
    '#description' => t('<em>example: '. l($base_url . $base_path .'rss.xml', 'rss.xml', array('attributes' => array('target' => '_blank'))) .'</em>'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fieldset_fastwebfeed']['fieldset_what_to_ping']['fieldset_default_rss_feed']['fastwebfeed_check_node_types_all_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('All node types'),
    '#description' => t('If checked, the "rss.xml" feed of "All node types" will be notified/pinged.'),
    '#default_value' => variable_get('fastwebfeed_check_node_types_all_admin', ''),
  );
  $form['fieldset_fastwebfeed']['fieldset_what_to_ping']['fieldset_default_rss_feed']['fastwebfeed_check_node_types_some_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('One of the following node types'),
    '#description' => t('If checked, only the "rss.xml" feed of "One of the following node types" will be notified/pinged.'),
    '#default_value' => variable_get('fastwebfeed_check_node_types_some_admin', ''),
  );
  $form['fieldset_fastwebfeed']['fieldset_what_to_ping']['fieldset_default_rss_feed']['fastwebfeed_select_node_types_admin'] = array(
    '#type' => 'select',
    '#title' => t('Overview of node types'),
    '#description' => t('Select one of the following node types.<br/>
    <em style="color:red;">Attention: This effects the update notification of the "rss.xml" above.</em>'),
    '#default_value' => variable_get('fastwebfeed_select_node_types_admin', ''),
    '#options' => node_get_types('names'),
    '#multiple' => TRUE,
  );
  return $form;
}

/**
 * Extends the default rss.xml feed with:
 * - SUP protocol
 * - pubsubhubbub and the hubbub protocol
 * 
 * @return mixed
 *   a feed or page not found
 */
function fastwebfeed_handler_node_feed() {
  fastwebfeed_ob_start();
  node_feed();
  fastwebfeed_ob_end();
}

/**
 * Implementation of hook_nodeapi().
 * Publish the node update to each hub.
 */
function fastwebfeed_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  $check_ping_on_node = variable_get('fastwebfeed_check_ping_on_node_submission_admin', '');
  if ($check_ping_on_node == 1 && $node->promote == 1) {
    switch ($op) {
      case 'insert':
        if (module_exists(fastwebfeed_pubsubhubbub) || module_exists(fastwebfeed_sup)) {
        $feed_urls = fastwebfeed_get_feed_url($node, 'node');
        if (module_exists(fastwebfeed_pubsubhubbub)) {
          if (!fastwebfeed_pubsubhubbub_fastwebfeed_ping($feed_urls)) {
            drupal_set_message(t("Pubsubhubbub warning: no feed URL's"), 'warning');
          }
        }
        if (module_exists(fastwebfeed_sup)) {
          if (!fastwebfeed_sup_fastwebfeed_ping($feed_urls)) {
            drupal_set_message(t("SUP warning: no feed URL's"), 'warning');
          }
        }
        module_invoke_all('fastwebfeed_path', '', 'reset all');
      }
      break;
    }
  }
}

/**
 * Implementation of the hook_fastwebfeed_path()
 * 
 * @param $node
 * 		node object
 * @param $type
 *    type of return
 * @return $feed_urls
 * 		array of fastwebfeed paths
 */
function fastwebfeed_fastwebfeed_path($node, $type) {
  $feed_paths = array();
  switch ($type) {
    case 'reset all':
      $feed_paths = _fastwebfeed_fastwebfeed_path_reset_all();
    break;
    case 'all':
      $feed_paths = _fastwebfeed_fastwebfeed_path_all();
    break;
    case 'node':
      $feed_paths = _fastwebfeed_fastwebfeed_path_node($node);
    break;
  }
  $feed_paths = array_unique($feed_paths);
  return $feed_paths;
}

/**
 * Returns all "taxonomy" fastwebfeed paths (and/or default rss.xml) to the the hook_fastwebfeed_path()
 * 
 * @return $feed_paths
 * 	Returns an array of feed paths
 * 
 * @see fastwebfeed_fastwebfeed_path()
 */
function _fastwebfeed_fastwebfeed_path_all() {
      $feed_paths = array();
      $feed_paths = variable_get('fastwebfeed_feed_paths', '');
      if (empty($feed_paths)) {
        fastwebfeed_fastwebfeed_path('', 'reset all');
        $feed_paths = variable_get('fastwebfeed_feed_paths', '');
      }
      return $feed_paths;
}

/**
 * Resets all "taxonomy" fastwebfeed paths (and/or default rss.xml) and
 * returns all "taxonomy" fastwebfeed paths (and/or default rss.xml) to the the hook_fastwebfeed_path()
 * 
 * @return $feed_paths
 * 	Returns an array of feed paths
 * 
 * @see fastwebfeed_fastwebfeed_path()
 */
function _fastwebfeed_fastwebfeed_path_reset_all() {
      $feed_paths = array();
      $feed_paths_ = array();
      $node = array();
      $result = db_query('SELECT n.nid, n.vid, n.type FROM {node} n');
      while ($node = db_fetch_object($result)) {
        $feed_paths_ = _fastwebfeed_fastwebfeed_path_node($node);
        foreach ($feed_paths_ as $feed_path_) {
          $feed_paths[] = $feed_path_;
        }
      }
      $feed_paths = array_unique($feed_paths);
      variable_set('fastwebfeed_feed_paths', $feed_paths);
      return $feed_paths;
}

/**
 * Returns specific "taxonomy" fastwebfeed paths (and/or default rss.xml) to the the hook_fastwebfeed_path()
 * 
 * @return $feed_paths
 * 	Returns an array of feed paths
 * 
 * @see fastwebfeed_fastwebfeed_path()
 */
function _fastwebfeed_fastwebfeed_path_node($node) {
  $feed_paths = array();
  $check_ping_on_node = variable_get('fastwebfeed_check_ping_on_node_submission_admin', '');
  $check_node_types_all = variable_get('fastwebfeed_check_node_types_all_admin', '');
  $check_node_types_some = variable_get('fastwebfeed_check_node_types_some_admin', '');
  if ($check_ping_on_node == 1) {
      if ($check_node_types_all == 1) {
        $feed_paths[] = 'rss.xml';
      }
      elseif ($check_node_types_some == 1) {
        $select_node_types = variable_get('fastwebfeed_select_node_types_admin', '');
        foreach ($select_node_types as $key_select_node_type => $value_select_node_type) {
          if ($node->type == $key_select_node_type) {
            $feed_paths[] = 'rss.xml';
          }
        }
      }
  }
  return $feed_paths;
}