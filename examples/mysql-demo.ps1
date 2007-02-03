param([string]$Exe = '.\bin\Release\SCC.exe')

$ErrorActionPreference = 'Stop'
$container = 'prueba-scc-mysql'
$image = 'mysql@sha256:a81d043930e51745522bd5c430961cf8286f0a23f0a9a2a6e0108f7ea71b0902'
$names = @('MYSQL_ROOT_PASSWORD','SCC_DB_HOST','SCC_DB_PORT','SCC_DB_USER','SCC_DB_PASSWORD')
$previous = @{}
foreach ($name in $names) { $previous[$name] = [Environment]::GetEnvironmentVariable($name,'Process') }
$owned = ''
try {
    $env:MYSQL_ROOT_PASSWORD = [Guid]::NewGuid().ToString('N')
    $env:SCC_DB_PASSWORD = [Guid]::NewGuid().ToString('N')
    $env:SCC_DB_USER = 'scc_demo'
    $env:SCC_DB_HOST = '127.0.0.1'
    $owned = docker run --rm -d --name $container -p '127.0.0.1::3306' -e MYSQL_DATABASE=scc -e MYSQL_ROOT_PASSWORD $image --mysql-native-password=ON --character-set-server=latin1 --collation-server=latin1_swedish_ci
    if ($LASTEXITCODE -ne 0 -or $owned -notmatch '^[a-f0-9]{64}$') { throw 'The disposable MySQL container did not start.' }
    $ready = 'MYSQL_PWD="$MYSQL_ROOT_PASSWORD" exec mysql --protocol=TCP --host=127.0.0.1 --user=root --database=scc --execute "SELECT 1"'
    $attempt = 0
    do {
        docker exec $owned sh -c $ready 2>$null | Out-Null
        $joined = $LASTEXITCODE -eq 0
        if (-not $joined) { Start-Sleep -Milliseconds 750 }
        $attempt++
    } while (-not $joined -and $attempt -lt 45)
    if (-not $joined) { throw 'The disposable MySQL instance did not become ready.' }
    $port = docker port $owned '3306/tcp'
    if ($port -notmatch '^127\.0\.0\.1:(\d+)$') { throw 'Expected a loopback-only address.' }
    $env:SCC_DB_PORT = $Matches[1]
    $schema = Get-Content -LiteralPath (Join-Path $PSScriptRoot 'schema.sql') -Raw
    $seed = Get-Content -LiteralPath (Join-Path $PSScriptRoot 'fictional-data.sql') -Raw
    $grant = "CREATE USER 'scc_demo'@'%' IDENTIFIED WITH mysql_native_password BY '$env:SCC_DB_PASSWORD';`nGRANT SELECT, INSERT, UPDATE ON scc.* TO 'scc_demo'@'%';"
    $mysql = 'MYSQL_PWD="$MYSQL_ROOT_PASSWORD" exec mysql --user=root --database=scc'
    $schema + "`n" + $seed + "`n" + $grant | docker exec -i $owned sh -c $mysql
    if ($LASTEXITCODE -ne 0) { throw 'MySQL refused the fixture.' }
    $application = Start-Process -FilePath (Resolve-Path -LiteralPath $Exe).Path -Wait -PassThru
    if ($application.ExitCode -notin @(0,2)) { throw "SCC ended with exit $($application.ExitCode)." }
} finally {
    if ($owned -match '^[a-f0-9]{64}$') { docker stop --time 2 $owned | Out-Null }
    foreach ($name in $names) { [Environment]::SetEnvironmentVariable($name,$previous[$name],'Process') }
}
