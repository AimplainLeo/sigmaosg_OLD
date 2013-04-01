// SulParticleSystemDataPropertySheetQT.h

#ifndef __SULPARTICLESYSTEMDATAPROPERTYSHEETQT_H__
#define __SULPARTICLESYSTEMDATAPROPERTYSHEETQT_H__

#include "SulQtPropertySheet.h"
//#include "SulParticleSystemDataOsg.h"
#include <QtGui/QWidget>

class CSulParticleSystemDataPropertySheetQt : public QWidget
{
public:
								CSulParticleSystemDataPropertySheetQt( class CSulParticleSystemDataOsg* data );

	void						createPropertySheet();

	void						updateFromUI();
	void						updateToUI();

private:
	class CSulParticleSystemDataOsg*	m_data;

	CSulQtPropertySheet*		m_psGeneral;
	CSulQtPropertySheet*		m_psDebris;
	CSulQtPropertySheet*		m_psParticles;
	CSulQtPropertySheet*		m_psParticleSystem;
	CSulQtPropertySheet*		m_psModularEmitter;
	CSulQtPropertySheet*		m_psCounterRandomRate;
	CSulQtPropertySheet*		m_psSectorPlacer;
	CSulQtPropertySheet*		m_psShooterRadial;
	CSulQtPropertySheet*		m_psProgramFluid;
};

#endif // __SULPARTICLESYSTEMDATAPROPERTYSHEETQT_H__