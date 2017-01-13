var jsonata=function(){"use strict";function a(a){var b=!1;if("number"==typeof a){var c=parseFloat(a);if(b=!isNaN(c),b&&!isFinite(c))throw{message:"Number out of range",value:a,stack:(new Error).stack}}return b}function b(b){var c=!1;return Array.isArray(b)&&(c=0==b.filter(function(b){return!a(b)}).length),c}function c(a,b,c){var k,m=c.lookup("__evaluate_entry");switch(m&&m(a,b,c),a.type){case"path":k=d(a,b,c);break;case"binary":k=f(a,b,c);break;case"unary":k=g(a,b,c);break;case"name":k=h(a,b,c);break;case"literal":k=i(a,b,c);break;case"wildcard":k=j(a,b,c);break;case"descendant":k=l(a,b,c);break;case"condition":k=v(a,b,c);break;case"block":k=w(a,b,c);break;case"function":k=y(a,b,c);break;case"variable":k=x(a,b,c);break;case"lambda":k=A(a,b,c);break;case"partial":k=B(a,b,c)}a.hasOwnProperty("predicate")&&(k=e(a.predicate,k,c)),a.hasOwnProperty("group")&&(k=s(a.group,k,c));var n=c.lookup("__evaluate_exit");return n&&n(a,b,c,k),k}function d(a,b,d){var e,f,g=!1;"variable"===a[0].type?a[0].absolute=!0:"unary"===a[0].type&&"["===a[0].value&&(b=[null]);for(var h=0;h<a.length;h++){var i=a[h];i.keepArray===!0&&(g=!0);var j=[];if(e=void 0,f=i.absolute===!0?[b]:Array.isArray(b)?b:[b],a.length>1&&"literal"===i.type&&(i.type="name"),f.forEach(function(a){var b=c(i,a,d);"undefined"!=typeof b&&(Array.isArray(b)&&"["!==i.value?b.forEach(function(a){"undefined"!=typeof a&&j.push(a)}):j.push(b))}),1==j.length?e=g?j:j[0]:j.length>1&&(e=j),"undefined"==typeof e)break;b=e}return e}function e(d,e,f){var g,h=e,i=[];return d.forEach(function(d){if(Array.isArray(h)||(h=[h]),i=[],g=void 0,"literal"===d.type&&a(d.value)){var e=d.value;Number.isInteger(e)||(e=Math.floor(e)),e<0&&(e=h.length+e),g=h[e]}else h.forEach(function(e,g){var j=c(d,e,f);a(j)&&(j=[j]),b(j)?j.forEach(function(a){Number.isInteger(a)||(a=Math.floor(a)),a<0&&(a=h.length+a),a==g&&i.push(e)}):X(j)&&i.push(e)});1==i.length?g=i[0]:i.length>1&&(g=i),h=g}),g}function f(a,b,c){var d;switch(a.value){case"+":case"-":case"*":case"/":case"%":d=n(a,b,c);break;case"=":case"!=":case"<":case"<=":case">":case">=":d=o(a,b,c);break;case"&":d=r(a,b,c);break;case"and":case"or":d=q(a,b,c);break;case"..":d=t(a,b,c);break;case":=":d=u(a,b,c);break;case"in":d=p(a,b,c)}return d}function g(b,d,e){var f;switch(b.value){case"-":if(f=c(b.expression,d,e),!a(f))throw{message:"Cannot negate a non-numeric value: "+f,stack:(new Error).stack,position:b.position,token:b.value,value:f};f=-f;break;case"[":f=[],b.lhs.forEach(function(a){var b=c(a,d,e);"undefined"!=typeof b&&("["===a.value?f.push(b):f=ba(f,b))});break;case"{":f=s(b,d,e)}return f}function h(a,b,c){var d;if(Array.isArray(b)){var e=[];b.forEach(function(b){var d=h(a,b,c);"undefined"!=typeof d&&e.push(d)}),1==e.length?d=e[0]:e.length>1&&(d=e)}else null!==b&&"object"==typeof b&&(d=b[a.value]);return d}function i(a){return a.value}function j(a,b){var c,d=[];return null!==b&&"object"==typeof b&&Object.keys(b).forEach(function(a){var c=b[a];Array.isArray(c)?(c=k(c),d=ba(d,c)):d.push(c)}),1==d.length?c=d[0]:d.length>1&&(c=d),c}function k(a,b){return"undefined"==typeof b&&(b=[]),Array.isArray(a)?a.forEach(function(a){k(a,b)}):b.push(a),b}function l(a,b){var c,d=[];return"undefined"!=typeof b&&(m(b,d),c=1==d.length?d[0]:d),c}function m(a,b){Array.isArray(a)||b.push(a),Array.isArray(a)?a.forEach(function(a){m(a,b)}):null!==a&&"object"==typeof a&&Object.keys(a).forEach(function(c){m(a[c],b)})}function n(b,d,e){var f,g=c(b.lhs,d,e),h=c(b.rhs,d,e);if("undefined"==typeof g||"undefined"==typeof h)return f;if(!a(g))throw{message:"LHS of "+b.value+" operator must evaluate to a number",stack:(new Error).stack,position:b.position,token:b.value,value:g};if(!a(h))throw{message:"RHS of "+b.value+" operator must evaluate to a number",stack:(new Error).stack,position:b.position,token:b.value,value:h};switch(b.value){case"+":f=g+h;break;case"-":f=g-h;break;case"*":f=g*h;break;case"/":f=g/h;break;case"%":f=g%h}return f}function o(a,b,d){var e,f=c(a.lhs,b,d),g=c(a.rhs,b,d);if("undefined"==typeof f||"undefined"==typeof g)return!1;switch(a.value){case"=":e=f==g;break;case"!=":e=f!=g;break;case"<":e=f<g;break;case"<=":e=f<=g;break;case">":e=f>g;break;case">=":e=f>=g}return e}function p(a,b,d){var e=!1,f=c(a.lhs,b,d),g=c(a.rhs,b,d);if("undefined"==typeof f||"undefined"==typeof g)return!1;Array.isArray(g)||(g=[g]);for(var h=0;h<g.length;h++)if(g[h]===f){e=!0;break}return e}function q(a,b,d){var e;switch(a.value){case"and":e=X(c(a.lhs,b,d))&&X(c(a.rhs,b,d));break;case"or":e=X(c(a.lhs,b,d))||X(c(a.rhs,b,d))}return e}function r(a,b,d){var e,f=c(a.lhs,b,d),g=c(a.rhs,b,d),h="",i="";return"undefined"!=typeof f&&(h=N(f)),"undefined"!=typeof g&&(i=N(g)),e=h.concat(i)}function s(a,b,d){var e={},f={};Array.isArray(b)||(b=[b]),b.forEach(function(b){a.lhs.forEach(function(e){var g=c(e[0],b,d);if("string"!=typeof g)throw{message:"Key in object structure must evaluate to a string. Got: "+g,stack:(new Error).stack,position:a.position,value:g};var h={data:b,expr:e[1]};f.hasOwnProperty(g)?f[g].data=ba(f[g].data,b):f[g]=h})});for(var g in f){var h=f[g],i=c(h.expr,h.data,d);e[g]=i}return e}function t(a,b,d){var e,f=c(a.lhs,b,d),g=c(a.rhs,b,d);if("undefined"==typeof f||"undefined"==typeof g)return e;if(f>g)return e;if(!Number.isInteger(f))throw{message:"LHS of range operator (..) must evaluate to an integer",stack:(new Error).stack,position:a.position,token:a.value,value:f};if(!Number.isInteger(g))throw{message:"RHS of range operator (..) must evaluate to an integer",stack:(new Error).stack,position:a.position,token:a.value,value:g};e=new Array(g-f+1);for(var h=f,i=0;h<=g;h++,i++)e[i]=h;return e}function u(a,b,d){var e=c(a.rhs,b,d);if("variable"!==a.lhs.type)throw{message:"Left hand side of := must be a variable name (start with $)",stack:(new Error).stack,position:a.position,token:a.value,value:"path"===a.lhs.type?a.lhs[0].value:a.lhs.value};return d.bind(a.lhs.value,e),e}function v(a,b,d){var e,f=c(a.condition,b,d);return X(f)?e=c(a.then,b,d):"undefined"!=typeof a.else&&(e=c(a.else,b,d)),e}function w(a,b,d){var e,f=ea(d);return a.expressions.forEach(function(a){e=c(a,b,f)}),e}function x(a,b,c){var d;return d=""===a.value?b:c.lookup(a.value)}function y(a,b,d){var e,f=[];a.arguments.forEach(function(a){f.push(c(a,b,d))});var g=c(a.procedure,b,d);if("undefined"==typeof g&&"path"===a.procedure.type&&d.lookup(a.procedure[0].value))throw{message:"Attempted to invoke a non-function. Did you mean '$"+a.procedure[0].value+"'?",stack:(new Error).stack,position:a.position,token:a.procedure[0].value};try{for(e=z(g,f,d,b);"object"==typeof e&&1==e.lambda&&1==e.thunk;){var h=c(e.body.procedure,e.input,e.environment);f=[],e.body.arguments.forEach(function(a){f.push(c(a,e.input,e.environment))}),e=z(h,f)}}catch(b){throw b.position=a.position,b.token="path"===a.procedure.type?a.procedure[0].value:a.procedure.value,b}return e}function z(a,b,c,d){var e;if(a&&a.lambda)e=C(a,b);else{if("function"!=typeof a)throw{message:"Attempted to invoke a non-function",stack:(new Error).stack};e=a.apply(d,b)}return e}function A(a,b,c){var d={lambda:!0,input:b,environment:c,arguments:a.arguments,body:a.body};return 1==a.thunk&&(d.thunk=!0),d}function B(a,b,d){var e,f=[];a.arguments.forEach(function(a){"operator"===a.type&&"?"===a.value?f.push(a):f.push(c(a,b,d))});var g=c(a.procedure,b,d);if("undefined"==typeof g&&"path"===a.procedure.type&&d.lookup(a.procedure[0].value))throw{message:"Attempted to partially apply a non-function. Did you mean '$"+a.procedure[0].value+"'?",stack:(new Error).stack,position:a.position,token:a.procedure[0].value};if(g&&g.lambda)e=D(g,f);else{if("function"!=typeof g)throw{message:"Attempted to partially apply a non-function",stack:(new Error).stack,position:a.position,token:"path"===a.procedure.type?a.procedure[0].value:a.procedure.value};e=E(g,f)}return e}function C(a,b){var d,e=ea(a.environment);return a.arguments.forEach(function(a,c){e.bind(a.value,b[c])}),d="function"==typeof a.body?F(a.body,e):c(a.body,a.input,e)}function D(a,b){var c=ea(a.environment),d=[];a.arguments.forEach(function(a,e){var f=b[e];f&&"operator"===f.type&&"?"===f.value?d.push(a):c.bind(a.value,f)});var e={lambda:!0,input:a.input,environment:c,arguments:d,body:a.body};return e}function E(a,b){var c=G(a);c=c.map(function(a){return"$"+a.trim()});var d="function("+c.join(", ")+"){ _ }",e=ja(d);e.body=a;var f=D(e,b);return f}function F(a,b){var c=G(a),d=c.map(function(a){return b.lookup(a.trim())}),e=a.apply(null,d);return e}function G(a){var b=a.toString(),c=/\(([^\)]*)\)/.exec(b)[1],d=c.split(",");return d}function H(a){var b=!1;return a&&"object"==typeof a&&a.lambda===!0&&a.hasOwnProperty("input")&&a.hasOwnProperty("arguments")&&a.hasOwnProperty("environment")&&a.hasOwnProperty("body")&&(b=!0),b}function I(a){var b=0;if(1!=arguments.length)throw{message:"The sum function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){Array.isArray(a)||(a=[a]);var c=a.filter(function(a){return"number"!=typeof a});if(c.length>0)throw{message:"Type error: argument of sum function must be an array of numbers",stack:(new Error).stack,value:c};return a.forEach(function(a){b+=a}),b}}function J(a){if(1!=arguments.length)throw{message:"The count function expects one argument",stack:(new Error).stack};return"undefined"==typeof a?0:(Array.isArray(a)||(a=[a]),a.length)}function K(a){var b;if(1!=arguments.length)throw{message:"The max function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){Array.isArray(a)||(a=[a]);var c=a.filter(function(a){return"number"!=typeof a});if(c.length>0)throw{message:"Type error: argument of max function must be an array of numbers",stack:(new Error).stack,value:c};return b=Math.max.apply(Math,a)}}function L(a){var b;if(1!=arguments.length)throw{message:"The min function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){Array.isArray(a)||(a=[a]);var c=a.filter(function(a){return"number"!=typeof a});if(c.length>0)throw{message:"Type error: argument of min function must be an array of numbers",stack:(new Error).stack,value:c};return b=Math.min.apply(Math,a)}}function M(a){var b=0;if(1!=arguments.length)throw{message:"The average function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){Array.isArray(a)||(a=[a]);var c=a.filter(function(a){return"number"!=typeof a});if(c.length>0)throw{message:"Type error: argument of average function must be an array of numbers",stack:(new Error).stack,value:c};return a.forEach(function(a){b+=a}),b/a.length}}function N(b){var c;if(1!=arguments.length)throw{message:"The string function expects one argument",stack:(new Error).stack};if("undefined"!=typeof b){if("string"==typeof b)c=b;else if("function"==typeof b||H(b))c="";else{if("number"==typeof b&&!isFinite(b))throw{message:"Attempting to invoke string function on Infinity or NaN",value:b,stack:(new Error).stack};c=JSON.stringify(b,function(b,c){return"undefined"!=typeof c&&null!==c&&c.toPrecision&&a(c)?Number(c.toPrecision(13)):c&&H(c)?"":"function"==typeof c?"":c})}return c}}function O(a,b,c){if(2!=arguments.length&&3!=arguments.length)throw{message:"The substring function expects two or three arguments",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: first argument of substring function must evaluate to a string",stack:(new Error).stack,value:a};if("number"!=typeof b)throw{message:"Type error: second argument of substring function must evaluate to a number",stack:(new Error).stack,value:b};if("undefined"!=typeof c&&"number"!=typeof c)throw{message:"Type error: third argument of substring function must evaluate to a number",stack:(new Error).stack,value:c};return a.substr(b,c)}}function P(a,b){if(2!=arguments.length)throw{message:"The substringBefore function expects two arguments",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: first argument of substringBefore function must evaluate to a string",stack:(new Error).stack,value:a};if("string"!=typeof b)throw{message:"Type error: second argument of substringBefore function must evaluate to a string",stack:(new Error).stack,value:b};var c=a.indexOf(b);return c>-1?a.substr(0,c):a}}function Q(a,b){if(2!=arguments.length)throw{message:"The substringAfter function expects two arguments",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: first argument of substringAfter function must evaluate to a string",stack:(new Error).stack,value:a};if("string"!=typeof b)throw{message:"Type error: second argument of substringAfter function must evaluate to a string",stack:(new Error).stack,value:b};var c=a.indexOf(b);return c>-1?a.substr(c+b.length):a}}function R(a){if(1!=arguments.length)throw{message:"The lowercase function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: argument of lowercase function must evaluate to a string",stack:(new Error).stack,value:a};return a.toLowerCase()}}function S(a){if(1!=arguments.length)throw{message:"The uppercase function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: argument of uppercase function must evaluate to a string",stack:(new Error).stack,value:a};return a.toUpperCase()}}function T(a){if(1!=arguments.length)throw{message:"The length function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: argument of length function must evaluate to a string",stack:(new Error).stack,value:a};return a.length}}function U(a,b,c){if(2!=arguments.length&&3!=arguments.length)throw{message:"The split function expects two or three arguments",stack:(new Error).stack};if("undefined"!=typeof a){if("string"!=typeof a)throw{message:"Type error: first argument of split function must evaluate to a string",stack:(new Error).stack,value:a};if("string"!=typeof b)throw{message:"Type error: second argument of split function must evaluate to a string",stack:(new Error).stack,value:b};if("undefined"!=typeof c&&("number"!=typeof c||c<0))throw{message:"Type error: third argument of split function must evaluate to a positive number",stack:(new Error).stack,value:c};return a.split(b,c)}}function V(a,b){if(1!=arguments.length&&2!=arguments.length)throw{message:"The join function expects one or two arguments",stack:(new Error).stack};if("undefined"!=typeof a){Array.isArray(a)||(a=[a]);var c=a.filter(function(a){return"string"!=typeof a});if(c.length>0)throw{message:"Type error: first argument of join function must be an array of strings",stack:(new Error).stack,value:c};if("undefined"==typeof b&&(b=""),"string"!=typeof b)throw{message:"Type error: second argument of split function must evaluate to a string",stack:(new Error).stack,value:b};return a.join(b)}}function W(a){var b;if(1!=arguments.length)throw{message:"The number function expects one argument",stack:(new Error).stack};if("undefined"!=typeof a){if("number"==typeof a)b=a;else{if("string"!=typeof a||!/^-?(0|([1-9][0-9]*))(\.[0-9]+)?([Ee][-+]?[0-9]+)?$/.test(a)||isNaN(parseFloat(a))||!isFinite(a))throw{message:"Unable to cast value to a number",value:a,stack:(new Error).stack};b=parseFloat(a)}return b}}function X(b){if(1!=arguments.length)throw{message:"The boolean function expects one argument",stack:(new Error).stack};if("undefined"!=typeof b){var c=!1;if(Array.isArray(b)){if(1==b.length)c=X(b[0]);else if(b.length>1){var d=b.filter(function(a){return X(a)});c=d.length>0}}else"string"==typeof b?b.length>0&&(c=!0):a(b)?0!=b&&(c=!0):null!=b&&"object"==typeof b?Object.keys(b).length>0&&(H(b)||(c=!0)):"boolean"==typeof b&&1==b&&(c=!0);return c}}function Y(a){return!X(a)}function Z(a){for(var b=arguments,c=[],d=[],e=1;e<b.length;e++)Array.isArray(b[e])?d.push(b[e]):d.push([b[e]]);if(d.length>0)for(var f=0;f<d[0].length;f++){for(var g=[],h=0;h<a.arguments.length;h++)g.push(d[h][f]);c.push(z(a,g,null,null))}return c}function $(a,b,c){var d;if(2!=a.length&&2!=a.arguments.length)throw{message:"The first argument of the reduce function must be a function of arity 2",stack:(new Error).stack};Array.isArray(b)||(b=[b]);var e;for("undefined"==typeof c&&b.length>0?(d=b[0],e=1):(d=c,e=0);e<b.length;)d=z(a,[d,b[e]],null,null),e++;return d}function _(a){var b=[];if(Array.isArray(a)){var c={};a.forEach(function(a){var b=_(a);Array.isArray(b)&&b.forEach(function(a){c[a]=!0})}),b=_(c)}else null==a||"object"!=typeof a||H(a)?b=void 0:(b=Object.keys(a),0==b.length&&(b=void 0));return b}function aa(a,b){var c=h({value:b},a);return c}function ba(a,b){return"undefined"==typeof a?b:"undefined"==typeof b?a:(Array.isArray(a)||(a=[a]),Array.isArray(b)||(b=[b]),Array.prototype.push.apply(a,b),a)}function ca(a){if(1!=arguments.length)throw{message:"The exists function expects one argument",stack:(new Error).stack};return"undefined"!=typeof a}function da(a){var b=[];if(Array.isArray(a))a.forEach(function(a){b=ba(b,da(a))});else if(null==a||"object"!=typeof a||H(a))b=a;else for(var c in a){var d={};d[c]=a[c],b.push(d)}return b}function ea(a){var b={};return{bind:function(a,c){b[a]=c},lookup:function(c){var d=b[c];return"undefined"==typeof d&&a&&(d=a.lookup(c)),d}}}function fa(a){var b=ja(a),d=ea(ka);return{evaluate:function(a,e){if("undefined"!=typeof e){var f;f=ea(d);for(var g in e)f.bind(g,e[g])}else f=d;return f.bind("$",a),c(b,a,f)},assign:function(a,b){d.bind(a,b)}}}var ga={".":75,"[":80,"]":0,"{":70,"}":0,"(":80,")":0,",":0,"@":75,"#":70,";":80,":":80,"?":20,"+":50,"-":50,"*":60,"/":60,"%":60,"|":20,"=":40,"<":40,">":40,"`":80,"**":60,"..":20,":=":10,"!=":40,"<=":40,">=":40,and:30,or:25,in:40,"&":50,"!":0},ha={'"':'"',"\\":"\\","/":"/",b:"\b",f:"\f",n:"\n",r:"\r",t:"\t"},ia=function(a){var b=0,c=a.length,d=function(a,c){var d={type:a,value:c,position:b};return d},e=function(){if(b>=c)return null;for(var e=a.charAt(b);b<c&&" \t\n\r\v".indexOf(e)>-1;)b++,e=a.charAt(b);if("."===e&&"."===a.charAt(b+1))return b+=2,d("operator","..");if(":"===e&&"="===a.charAt(b+1))return b+=2,d("operator",":=");if("!"===e&&"="===a.charAt(b+1))return b+=2,d("operator","!=");if(">"===e&&"="===a.charAt(b+1))return b+=2,d("operator",">=");if("<"===e&&"="===a.charAt(b+1))return b+=2,d("operator","<=");if("*"===e&&"*"===a.charAt(b+1))return b+=2,d("operator","**");if(ga.hasOwnProperty(e))return b++,d("operator",e);if('"'===e||"'"===e){var f=e;b++;for(var g="";b<c;){if(e=a.charAt(b),"\\"===e)if(b++,e=a.charAt(b),ha.hasOwnProperty(e))g+=ha[e];else{if("u"!==e)throw{message:"unsupported escape sequence: \\"+e,stack:(new Error).stack,position:b,token:e};var h=a.substr(b+1,4);if(!/^[0-9a-fA-F]+$/.test(h))throw{message:"The escape sequence \\u must be followed by 4 hex digits",stack:(new Error).stack,position:b};var i=parseInt(h,16);g+=String.fromCharCode(i),b+=4}else{if(e===f)return b++,d("string",g);g+=e}b++}throw{message:"no terminating quote found in string literal",stack:(new Error).stack,position:b}}var j=/^-?(0|([1-9][0-9]*))(\.[0-9]+)?([Ee][-+]?[0-9]+)?/,k=j.exec(a.substring(b));if(null!==k){var l=parseFloat(k[0]);if(!isNaN(l)&&isFinite(l))return b+=k[0].length,d("number",l);throw{message:"Number out of range: "+k[0],stack:(new Error).stack,position:b,token:k[0]}}for(var m,n,o=b;;)if(m=a.charAt(o),o==c||" \t\n\r\v".indexOf(m)>-1||ga.hasOwnProperty(m)){if("$"===a.charAt(b))return n=a.substring(b+1,o),b=o,d("variable",n);switch(n=a.substring(b,o),b=o,n){case"and":case"or":case"in":return d("operator",n);case"true":return d("value",!0);case"false":return d("value",!1);case"null":return d("value",null);default:return b==c&&""===n?null:d("name",n)}}else o++};return e},ja=function(b){var c,d,e={},f={nud:function(){return this}},g=function(a,b){var c=e[a];return b=b||0,c?b>=c.lbp&&(c.lbp=b):(c=Object.create(f),c.id=c.value=a,c.lbp=b,e[a]=c),c},h=function(a){if(a&&c.id!==a){var b;throw b="(end)"===c.id?"Syntax error: expected '"+a+"' before end of expression":"Syntax error: expected '"+a+"', got '"+c.id,{message:b,stack:(new Error).stack,position:c.position,token:c.id,value:a}}var f=d();if(null===f)return c=e["(end)"];var g,h=f.value,i=f.type;switch(i){case"name":case"variable":g=e["(name)"];break;case"operator":if(g=e[h],!g)throw{message:"Unknown operator: "+h,stack:(new Error).stack,position:f.position,token:h};break;case"string":case"number":case"value":i="literal",g=e["(literal)"];break;default:throw{message:"Unexpected token:"+h,stack:(new Error).stack,position:f.position,token:h}}return c=Object.create(g),c.value=h,c.type=i,c.position=f.position,c},i=function(a){var b,d=c;for(h(),b=d.nud();a<c.lbp;)d=c,h(),b=d.led(b);return b},j=function(a,b,c){var d=b||ga[a],e=g(a,d);return e.led=c||function(a){return this.lhs=a,this.rhs=i(d),this.type="binary",this},e},k=function(a,b,c){var d=b||ga[a],e=g(a,d);return e.led=c||function(a){return this.lhs=a,this.rhs=i(d-1),this.type="binary",this},e},l=function(a,b){var c=g(a);return c.nud=b||function(){return this.expression=i(70),this.type="unary",this},c};g("(end)"),g("(name)"),g("(literal)"),g(":"),g(";"),g(","),g(")"),g("]"),g("}"),g(".."),j("."),j("+"),j("-"),j("*"),j("/"),j("%"),j("="),j("<"),j(">"),j("!="),j("<="),j(">="),j("&"),j("and"),j("or"),j("in"),k(":="),l("-"),l("*",function(){return this.type="wildcard",this}),l("**",function(){return this.type="descendant",this}),j("(",ga["("],function(a){if(this.procedure=a,this.type="function",this.arguments=[],")"!==c.id)for(;"operator"===c.type&&"?"===c.id?(this.type="partial",this.arguments.push(c),h("?")):this.arguments.push(i(0)),","===c.id;)h(",");return h(")"),"name"!==a.type||"function"!==a.value&&"λ"!==a.value||(this.arguments.forEach(function(a,b){if("variable"!==a.type)throw{message:"Parameter "+(b+1)+" of function definition must be a variable name (start with $)",stack:(new Error).stack,position:a.position,token:a.value}}),this.type="lambda",h("{"),this.body=i(0),h("}")),this}),l("(",function(){for(var a=[];")"!==c.id&&(a.push(i(0)),";"===c.id);)h(";");return h(")"),this.type="block",this.expressions=a,this}),l("[",function(){var a=[];if("]"!==c.id)for(;;){var b=i(0);if(".."===c.id){var d={type:"binary",value:"..",position:c.position,lhs:b};h(".."),d.rhs=i(0),b=d}if(a.push(b),","!==c.id)break;h(",")}return h("]"),this.lhs=a,this.type="unary",this}),j("[",ga["["],function(a){if("]"===c.id){for(var b=a;b&&"binary"===b.type&&"["===b.value;)b=b.lhs;return b.keepArray=!0,h("]"),a}return this.lhs=a,this.rhs=i(ga["]"]),this.type="binary",h("]"),this});var m=function(a){var b=[];if("}"!==c.id)for(;;){var d=i(0);h(":");var e=i(0);if(b.push([d,e]),","!==c.id)break;h(",")}return h("}"),"undefined"==typeof a?(this.lhs=b,this.type="unary"):(this.lhs=a,this.rhs=b,this.type="binary"),this};l("{",m),j("{",ga["{"],m),j("?",ga["?"],function(a){return this.type="condition",this.condition=a,this.then=i(0),":"===c.id&&(h(":"),this.else=i(0)),this});var n=function(a){var b;if("function"===a.type){var c={type:"lambda",thunk:!0,arguments:[],position:a.position};c.body=a,b=c}else if("condition"===a.type)a.then=n(a.then),a.else=n(a.else),b=a;else if("block"===a.type){var d=a.expressions.length;d>0&&(a.expressions[d-1]=n(a.expressions[d-1])),b=a}else b=a;return b},o=function(b){var c=[];switch(b.type){case"binary":switch(b.value){case".":var d=o(b.lhs);"path"===d.type?Array.prototype.push.apply(c,d):c.push(d);var e=o(b.rhs);"path"!==e.type&&(e=[e]),Array.prototype.push.apply(c,e),c.type="path";break;case"[":c=o(b.lhs);var f=c;if("path"===c.type&&(f=c[c.length-1]),"undefined"!=typeof f.group)throw{message:"A predicate cannot follow a grouping expression in a step. Error at column: "+b.position,stack:(new Error).stack,position:b.position};"undefined"==typeof f.predicate&&(f.predicate=[]),f.predicate.push(o(b.rhs));break;case"{":if(c=o(b.lhs),"undefined"!=typeof c.group)throw{message:"Each step can only have one grouping expression. Error at column: "+b.position,stack:(new Error).stack,position:b.position};c.group={lhs:b.rhs.map(function(a){return[o(a[0]),o(a[1])]}),position:b.position};break;default:c={type:b.type,value:b.value,position:b.position},c.lhs=o(b.lhs),c.rhs=o(b.rhs)}break;case"unary":c={type:b.type,value:b.value,position:b.position},"["===b.value?c.lhs=b.lhs.map(function(a){return o(a)}):"{"===b.value?c.lhs=b.lhs.map(function(a){return[o(a[0]),o(a[1])]}):(c.expression=o(b.expression),"-"===b.value&&"literal"===c.expression.type&&a(c.expression.value)&&(c=c.expression,c.value=-c.value));break;case"function":case"partial":c={type:b.type,name:b.name,value:b.value,position:b.position},c.arguments=b.arguments.map(function(a){return o(a)}),c.procedure=o(b.procedure);break;case"lambda":c={type:b.type,arguments:b.arguments,position:b.position};var g=o(b.body);c.body=n(g);break;case"condition":c={type:b.type,position:b.position},c.condition=o(b.condition),c.then=o(b.then),"undefined"!=typeof b.else&&(c.else=o(b.else));break;case"block":c={type:b.type,position:b.position},c.expressions=b.expressions.map(function(a){return o(a)});break;case"name":c=[b],c.type="path";break;case"literal":case"wildcard":case"descendant":case"variable":c=b;break;case"operator":if("and"===b.value||"or"===b.value||"in"===b.value)b.type="name",c=o(b);else{if("?"!==b.value)throw{message:"Syntax error: "+b.value,stack:(new Error).stack,position:b.position,token:b.value};c=b}break;default:var h="Unknown expression type: "+b.value;throw"(end)"===b.id&&(h="Syntax error: unexpected end of expression"),{message:h,stack:(new Error).stack,position:b.position,token:b.value}}return c};d=ia(b),h();var p=i(0);if("(end)"!==c.id)throw{message:"Syntax error: "+c.value,stack:(new Error).stack,position:c.position,token:c.value};return p=o(p)};Number.isInteger=Number.isInteger||function(a){return"number"==typeof a&&isFinite(a)&&Math.floor(a)===a};var ka=ea(null);return ka.bind("sum",I),ka.bind("count",J),ka.bind("max",K),ka.bind("min",L),ka.bind("average",M),ka.bind("string",N),ka.bind("substring",O),ka.bind("substringBefore",P),ka.bind("substringAfter",Q),ka.bind("lowercase",R),ka.bind("uppercase",S),ka.bind("length",T),ka.bind("split",U),ka.bind("join",V),ka.bind("number",W),ka.bind("boolean",X),ka.bind("not",Y),ka.bind("map",Z),ka.bind("reduce",$),ka.bind("keys",_),ka.bind("lookup",aa),ka.bind("append",ba),ka.bind("exists",ca),ka.bind("spread",da),fa.parser=ja,fa}();"undefined"!=typeof module&&(module.exports=jsonata),function(){function a(a,b){if(b<=0)return a;var c=new Array(b).join(" ");return a=a.replace(/^\s*/,c)}function b(b){for(var c,d,e=b.length,f=!1,g=[],h=[],i={"(":")","[":"]","{":"}"},j=0;j<e;j++){var k=b[j];if(f)k===c&&(f=!1,h.pop());else if("'"===k||'"'===k)f=!0,c=k,d={type:"string",pos:j},g.push(d),h.push(d);else if(";"===k)d={type:";",pos:j},g.push(d);else if(","===k)d={type:",",pos:j},g.push(d);else if(/[\(\[\{]/.test(k))d={type:"open-block",char:k,pos:j},g.push(d),h.push(d);else if(/[\}\)\]]/.test(k)){var l=h.pop();if(i[l.char]!==k)return b;l.width=j-l.pos,d={type:"close-block",pos:j,char:k,width:l.width},g.push(d)}}var m,n,o,p=b,q=0,r=0,s=[];return g.forEach(function(b){";"===b.type||","===b.type?s[s.length-1]&&(m=p.substring(0,r+b.pos+1),n=p.substring(r+b.pos+1),o=a(n,q),p=m+"\n"+o,r+=o.length-n.length+1):"open-block"===b.type?b.width>30?(s.push(!0),q+=4,m=p.substring(0,r+b.pos+1),n=p.substring(r+b.pos+1),o=a(n,q),p=m+"\n"+o,r+=o.length-n.length+1):s.push(!1):"close-block"===b.type&&(b.width>30&&(q-=4,m=p.substring(0,r+b.pos),n=p.substring(r+b.pos),o=a(n,q),p=m+"\n"+o,r+=o.length-n.length+1),s.pop())}),p}jsonata.format=b,jsonata.functions={$append:{args:["array","array"]},$average:{args:["value"]},$boolean:{args:["value"]},$count:{args:["array"]},$exists:{args:["value"]},$join:{args:["array","separator"]},$keys:{args:["object"]},$length:{args:["string"]},$lookup:{args:["object","key"]},$lowercase:{args:["string"]},$map:{args:[]},$max:{args:["array"]},$min:{args:["array"]},$not:{args:["value"]},$number:{args:["value"]},$reduce:{args:[]},$split:{args:["string","separator","limit"]},$spread:{args:["object"]},$string:{args:["value"]},$substring:{args:["string","start","length"]},$substringAfter:{args:["string","chars"]},$substringBefore:{args:["string","chars"]},$sum:{args:["array"]},$uppercase:{args:["string"]}},jsonata.getFunctionSnippet=function(a){var b="";if(jsonata.functions.hasOwnProperty(a)){var c=jsonata.functions[a];b="\\"+a+"(",c.args&&(b+=c.args.map(function(a,b){return"${"+(b+1)+":"+a+"}"}).join(", ")),b+=")\n"}return b}}();