<!--
# Copyright (c) FFLAS-FFPACK
# Written by Clément Pernet <clement.pernet@imag.fr>
# ========LICENCE========
# This file is part of the library FFLAS-FFPACK.
#
# FFLAS-FFPACK is free software: you can redistribute it and/or modify
# it under the terms of the  GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# ========LICENCE========
#/
-->


<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="html" encoding="ISO-8859-1" 
    doctype-public="-//W3C//DTD HTML 4.01//EN"
    doctype-system="http://www.w3.org/TR/html4/strict.dtd"
    indent="yes"/>

  <xsl:template match="benchmark">
    <html>
      <head>
        <link href="fflas.css" rel="stylesheet" type="text/css" />
        <title>FFLAS-FFPACK benchmark suite</title>
      </head>
      <body>
        <h1 align="center">FFLAS-FFPACK Benchmark suite</h1>
        <h2 align="center">Testing <xsl:value-of select="archi"/></h2>
        <p align="center">FFLAS-FFPACK computation is done over GF( <xsl:value-of select="prime"/>)</p>
        <xsl:for-each select="test">
          <hr/> 
          <p class="report">
            <h3><xsl:value-of select="./@name"/> report</h3>        
            <table border="1">
              <tr>
                <td class="headdimension"> Dimension </td>
                <xsl:for-each select="function[1]/run">
                  <td class="dimension"> <xsl:value-of select="./@size"/> </td>
                </xsl:for-each>    
              </tr>
              <xsl:for-each select="function">
                <tr>
                  <td class="headtime"> <xsl:value-of select="./@name"/>  with <xsl:value-of select="./@blas"/></td>
                  <xsl:for-each select="run">
                    <td class="time"> <xsl:value-of select='format-number(., "#.00")'/><i>s</i></td>
                  </xsl:for-each>                  
                </tr> 
              </xsl:for-each>
            </table>   
          </p>
        </xsl:for-each>
        <hr/>
        <p class="footnote"> page automatically generated - Pascal Giorgi @2007</p>
      </body>
    </html>
  </xsl:template>
  
</xsl:stylesheet>
