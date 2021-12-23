<?php
    include_once 'include/head.inc.php';
    //afficher les diférent comptes d'un client
    session_start();
?>

<table>
    <tr>
        <th>nom</th>
        <th>Prenom</th>
        <th> age</th>
    </tr>

    <tr>
        <td> Vincent</td>
        <td> Girard </td>
        <td> 20 ans </td>

    </tr>

    <tr>
        <td> Etame </td>
        <td> Cedric </td>
        <td> 23 ans </td>
    </tr>

    <tr>
        <td> Levesque </td>
        <td> Gautier </td>
        <td> 23 ans </td>
    </tr>

    <tr>
        <td> Mosquera </td>
        <td> Alexis </td>
        <td> 20 ans </td>
    </tr>

    <tr>
        <td> Vertus </td>
        <td> Makita </td>
        <td> 22 ans </td>
    </tr>
</table>

<?php
    include_once 'include/footeur.inc.php';
?>