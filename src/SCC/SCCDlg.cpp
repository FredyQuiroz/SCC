// SCCDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "SCC.h"
#include "SCCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cuadro de diálogo de CSCCDlg




CSCCDlg::CSCCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bExiste= false;
}

void CSCCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CCLIENTE, m_CBCliente);
	DDX_Control(pDX, IDC_EFACTURA, m_EFactura);
	DDX_Control(pDX, IDC_ESERVICIO, m_EServicio);
	DDX_Control(pDX, IDC_EREMISION, m_ERemision);
	DDX_Control(pDX, IDC_LDETALLE, m_LCReporte);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_BUTTON1, m_BCancelar);
	DDX_Control(pDX, IDC_BUTTON2, m_BTodos);
}

BEGIN_MESSAGE_MAP(CSCCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSCCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSCCDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_BN_CLICKED(IDC_BUTTON1, &CSCCDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_CCLIENTE, &CSCCDlg::OnCbnSelchangeCcliente)
	ON_NOTIFY(NM_DBLCLK, IDC_LDETALLE, &CSCCDlg::OnNMDblclkLdetalle)
	ON_EN_KILLFOCUS(IDC_ESERVICIO, &CSCCDlg::OnEnKillfocusEServicio)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CSCCDlg::OnBnClickedButton2)
	ON_EN_KILLFOCUS(IDC_EREMISION, &CSCCDlg::OnEnKillfocusEremision)
	ON_EN_KILLFOCUS(IDC_EFACTURA, &CSCCDlg::OnEnKillfocusEfactura)
END_MESSAGE_MAP()


// Controladores de mensaje de CSCCDlg

BOOL CSCCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// ShowWindow(SW_MINIMIZE);


	const char* host = getenv("SCC_DB_HOST");
	const char* user = getenv("SCC_DB_USER");
	const char* password = getenv("SCC_DB_PASSWORD");
	const char* port = getenv("SCC_DB_PORT");
	m_pConector.AddDataBase("scc", user ? user : "root", password ? password : "",
		host ? host : "localhost", port ? atoi(port) : 3306);


	m_LCReporte.InsertColumn(0, "Factura", 0, 100);
	m_LCReporte.InsertColumn(1, "Servicio", 0, 100);
	m_LCReporte.InsertColumn(2, "Remisión", 0, 100);
	m_LCReporte.InsertColumn(3, "Cliente", 0, 100);
	m_LCReporte.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCReporte.SetMessageLista("No hay facturas");

	OnBnClickedButton1();
	OnBnClickedButton2();
	return TRUE; 
}

void CSCCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono. Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CSCCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CSCCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CSCCDlg::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {
	
	CMySQLMessage mess(lParam);
	int idmess=mess.GetID();

	switch( wParam) {
		case QUERY_RESULT:
			if( idmess==query_cliente) {
				m_CBCliente.SetItemData(m_CBCliente.AddString(mess["nombre"]), atoi(mess["id"]));
			} else if( idmess==query_loadcomp) {
				int pos= m_LCReporte.GetItemCount();
				m_LCReporte.InsertItem(pos,mess["numero"]);
				m_LCReporte.Text[pos][1]=mess["servicio1"];
				m_LCReporte.Text[pos][2]=mess["remision"];
				m_LCReporte.Text[pos][3]=mess["nombre"];
				m_LCReporte.SetItemData(pos, atoi(mess["servicio2"]));
			} else if( idmess==query_modificacion) {
				m_bExiste=true;
				SetWindowText("SCC - Modificación");
				m_szID=mess["servicio2"];
				m_EFactura.Text=mess["numero"];
				m_EServicio.Text=mess["servicio1"];
				m_ERemision.Text=mess["remision"];
				m_CBCliente.Text=mess["nombre"];
			}
			break;
		case QUERY_EMPTY:
			if(idmess==3) {
				m_bExiste=false;
			}
			break;
		case QUERY_TITLE:
			if( idmess==1) {
				CString texto;
				texto=m_CBCliente.Text;
				m_CBCliente.ResetContent();
				m_CBCliente.Text=texto;
			}
			break;
		case QUERY_ERROR:
			MessageBox(mess[0], mess[1]);
			break;
	}
	return 0;
}

void CSCCDlg::OnBnClickedOk()
{
	CStringArray valores,nombres;
	CString temp,cliente;
	// cliente
	temp=m_CBCliente.Text;
	int pos=m_CBCliente.GetCurSelEx();
	if(temp.IsEmpty()) {
		MessageBox("Por favor ingrese o seleccione un cliente");
		m_CBCliente.SetFocus();
		return;
	}
	if(pos==-1) { // cliente nuevo
		valores.Add("");
		valores.Add(temp);
		if(m_pConector.Find(CQuery::CreateInsert("clientes", valores), this, &valores)!=-1) {
			cliente= valores.GetAt(0);
		} else return;
	} else {
		cliente.Format("%d", m_CBCliente.GetItemData(pos));
	}
	valores.RemoveAll();
	if(m_bExiste) {
		nombres.Add("numero");
		nombres.Add("servicio");
		nombres.Add("remision");
		nombres.Add("cliente");
	}
	// factura
	temp=m_EFactura.Text;
	valores.Add(temp);
	temp=m_EServicio.Text;
	if( temp.IsEmpty()) {
		MessageBox("Por favor ingrese un número de servicio");
		m_EServicio.SetFocus();
		return ;
	}

	valores.Add(temp);
	temp=m_ERemision.Text;
	if( temp.IsEmpty()) {
		MessageBox("Por favor ingrese una remision");
		m_ERemision.SetFocus();
		return ;
	}
	valores.Add(temp);
	valores.Add(cliente);
	if(m_bExiste) {
		m_pConector.Query(CQuery::CreateUpdate("facturas", nombres, valores, "servicio=" + m_szID), 100, this);
	} else m_pConector.Query(CQuery::CreateInsert("facturas", valores), 100, this);
	OnBnClickedButton1();
	// OnOK();
}

void CSCCDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CSCCDlg::OnBnClickedButton1()
{
	m_bExiste=false;
	m_szID.Empty();
	m_EFactura.Text="";
	m_ERemision.Text="";
	m_EServicio.Text="";
	m_LCReporte.DeleteAllItems();
	CQuery queryClientes("clientes");
	
	queryClientes["id"]="id";
	queryClientes["nombre"]="nombre";
	CString queryCli = queryClientes.GetSelect();
	m_pConector.Query(queryCli, query_cliente, this);

	//m_pConector.Query("SELECT id,nombre FROM clientes", 1, this);
	m_CBCliente.SetCurSel(-1);
	SetWindowText("SCC - Nuevo");
	OnCbnSelchangeCcliente();
}

void CSCCDlg::OnCbnSelchangeCcliente()
{
	int pos= m_CBCliente.GetCurSel();
	if( pos!=-1) {
		m_pConector.KillQuery(2, this);
		m_LCReporte.DeleteAllItems();
		pos= m_CBCliente.GetItemData(pos);
		
		CString condicion;
		condicion.Format("facturas.cliente=%d", pos);

		CQuery queryFacturas("facturas");
		queryFacturas["numero"]="facturas.numero";
		queryFacturas["servicio1"]="facturas.servicio";
		queryFacturas["remision"]="facturas.remision";

		queryFacturas["nombre"]="clientes.nombre";
		queryFacturas["servicio2"]="facturas.servicio";

		CString newquery=queryFacturas.GetSelect(condicion,"LEFT JOIN clientes ON clientes.id=facturas.cliente");
		m_pConector.Query(newquery, query_loadcomp, this);
		//query.Format("SELECT facturas.numero,facturas.servicio,facturas.remision,clientes.nombre,facturas.servicio FROM facturas LEFT JOIN clientes ON clientes.id=facturas.cliente WHERE facturas.cliente=%d", pos);
	}
}

void CSCCDlg::OnNMDblclkLdetalle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	int index= pNMListView->iItem;
	m_szID.Format("%d", m_LCReporte.GetItemData(index));
	m_szCurrent= m_LCReporte.Text[index][1];
	m_EFactura.Text=m_LCReporte.Text[index][0];
	m_EServicio.Text=m_LCReporte.Text[index][1];
	m_ERemision.Text=m_LCReporte.Text[index][2];
	m_CBCliente.Text=m_LCReporte.Text[index][3];
	m_bExiste=true;
	SetWindowText("SCC - Modificación");
	*pResult = 0;
}

void CSCCDlg::OnEnKillfocusEServicio() {
	CString temp,query,cliente;

	temp=m_EServicio.Text;
	if( temp.IsEmpty() || m_szCurrent==temp){
		m_szCurrent="";
		return;
	}

	m_szCurrent=temp;
	cliente=m_CBCliente.Text;
	OnBnClickedButton1();
	m_EServicio.Text=temp;
	m_CBCliente.Text=cliente;

	CString condicion;
	condicion.Format("facturas.servicio=%s", temp);

	CQuery queryFacturas("facturas");
	queryFacturas["numero"]="facturas.numero";
	queryFacturas["servicio1"]="facturas.servicio";
	queryFacturas["remision"]="facturas.remision";

	queryFacturas["nombre"]="clientes.nombre";
	queryFacturas["servicio2"]="facturas.servicio";

	CString newquery=queryFacturas.GetSelect(condicion,"LEFT JOIN clientes ON clientes.id=facturas.cliente");
	m_pConector.Query(newquery, query_modificacion, this);
	//query.Format("SELECT facturas.numero,facturas.servicio,facturas.remision,clientes.nombre,facturas.servicio FROM facturas LEFT JOIN clientes ON clientes.id=facturas.cliente WHERE facturas.servicio=%s", temp);
}

void CSCCDlg::OnEnKillfocusEfactura()
{
	/*
	if( !m_bExiste) {

		CString temp;
		temp=m_EFactura.Text;
		if( temp.IsEmpty())
			return;
		CStringArray valores;
		m_pConector.Find("SELECT numero FROM facturas WHERE numero='"+ temp+"'", this, &valores);
		if( !valores.IsEmpty()) {
			MessageBox("Factura '" + temp+ "' Existente");
			m_EFactura.Text="";
			m_EFactura.SetFocus();
			return;
		}
	}
	*/
}

void CSCCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(::IsWindow(m_LCReporte)) {
		CRect rect;
		m_LCReporte.GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom= cy-10;
		rect.right=cx-10;
		m_LCReporte.MoveWindow(rect);
	}
}

void CSCCDlg::OnBnClickedButton2()
{
	m_pConector.KillQuery(2, this);
	m_LCReporte.DeleteAllItems();
	
	CQuery queryFacturas("facturas");
	queryFacturas["numero"]="facturas.numero";
	queryFacturas["servicio1"]="facturas.servicio";
	queryFacturas["remision"]="facturas.remision";

	queryFacturas["nombre"]="clientes.nombre";
	queryFacturas["servicio2"]="facturas.servicio";

	CString newquery=queryFacturas.GetSelect("","LEFT JOIN clientes ON clientes.id=facturas.cliente");
	m_pConector.Query(newquery, query_loadcomp, this);
	
	//CString query;
	//query.Format("SELECT facturas.numero,facturas.servicio,facturas.remision,clientes.nombre,facturas.servicio FROM facturas LEFT JOIN clientes ON clientes.id=facturas.cliente");
	//m_pConector.Query(query, 2, this);
}

void CSCCDlg::OnEnKillfocusEremision()
{
	/*
	if( !m_bExiste) {

		CString temp;
		temp=m_ERemision.Text;
		if( temp.IsEmpty())
			return;
		CStringArray valores;
		m_pConector.Find("SELECT numero FROM facturas WHERE remision='"+ temp+"'", this, &valores);
		if( !valores.IsEmpty()) {
			MessageBox("Remision '" + temp+ "' Existente");
			m_ERemision.Text="";
			m_ERemision.SetFocus();
			return;
		}
	}*/
}
