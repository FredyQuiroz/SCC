// SCCDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include <Conector/Conector.h>

// Cuadro de diálogo de CSCCDlg
class CSCCDlg : public CDialog
{
	enum {
		query_cliente=1,
		query_loadcomp,
		query_modificacion
	};
	CConector	m_pConector;
	CString		m_szCurrent;
	bool		m_bExiste;
	CString		m_szID;
// Construcción
public:
	CSCCDlg(CWnd* pParent = NULL);	// Constructor estándar

// Datos del cuadro de diálogo
	enum { IDD = IDD_SCC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBoxXP m_CBCliente;
public:
	CEditXP m_EFactura;
public:
	CEditXP m_EServicio;
public:
	CEditXP m_ERemision;
public:
	CMyListCtrl m_LCReporte;
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnCbnSelchangeCcliente();
public:
	afx_msg void OnNMDblclkLdetalle(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnEnKillfocusEfactura();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	CButtonXP m_BTodos;
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnEnKillfocusEremision();
	afx_msg void OnEnKillfocusEServicio();
};
