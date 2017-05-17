<div id="form_<?php print check_plain($variables['account_id']); ?>">Loading...</div>
<script>/*<![CDATA[*/
  var script = document.createElement('script');
  var div = jQuery('.field-item'),
    comment = div.contents().filter(function() {
      return this.nodeType === 8;
    }).get(0);

  if (typeof comment !== "undefined") {
    var program = comment.nodeValue.split(":");
  } else {
    var program = "";
  }
  script.async = 1; script.src = 'https://grad.apply.colorado.edu/register/?id=<?php print check_plain($variables['account_id']); ?>&output=embed&sys:field:department=' + program[1] + '&div=form_<?php print check_plain($variables['account_id']); ?>' + ((location.search.length > 1) ? '&' + location.search.substring(1) : ''); var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(script, s);
  /*]]>*/</script>
