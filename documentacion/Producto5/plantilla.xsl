<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:output method="html" indent="yes" />

    <xsl:template match="/">
        <html>
            <head>
                <title>Adaptador de Red</title>
                <style>
                    body {
                        background-color: #BDF1CA;
                    }
                </style>
            </head>
            <body>
                <h1>Adaptador de Red</h1>
                <table border="1">
                    <tr style="background-color: #AAFFAA; color: blue; font-weight: bold;">
                        <th align="left">Nombre:</th>
                        <td><xsl:value-of select="adaptador/nombre" /></td>
                    </tr>
                    <tr>
                        <th align="left">IP Adaptador:</th>
                        <td><xsl:value-of select="adaptador/ip" /></td>
                    </tr>
                    <tr>
                        <th align="left">Mascara:</th>
                        <td><xsl:value-of select="adaptador/mascara" /></td>
                    </tr>
                    <tr>
                        <th align="left">Tipo DNS:</th>
                        <td><xsl:value-of select="adaptador/DNS/tipo" /></td>
                    </tr>
                    <tr>
                        <th align="left">IP DNS:</th>
                        <td><xsl:value-of select="adaptador/DNS/ip" /></td>
                    </tr>
                    <tr>
                        <th align="left">Tiempo Respuesta:</th>
                        <td>
                            <xsl:value-of select="adaptador/DNS/tiempo" />
                            <xsl:text> ms</xsl:text>
                        </td>
                    </tr>
                    <tr>
                        <th align="left">Número Saltos:</th>
                        <td><xsl:value-of select="adaptador/DNS/saltos" /></td>
                    </tr>
                    <tr>
                        <th align="left" valign="top">IP Saltos:</th>
                        <td>
                            <xsl:for-each select="adaptador/DNS/saltar/ip">
                                <xsl:choose>
                                    <xsl:when test=". = 'Desconocida'">
                                        <span style="color: red; font-weight: bold;">Desconocida</span>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:value-of select="." />
                                    </xsl:otherwise>
                                </xsl:choose>
                                <br/>
                            </xsl:for-each>
                        </td>
                    </tr>
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
