/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _TPLCOMP_HXX
#define _TPLCOMP_HXX

#include <osl/mutex.hxx>
//ASDBG #ifndef _USR_UNO_HXX
//ASDBG #include <usr/uno.hxx>
//ASDBG #endif
//ASDBG #ifndef _USR_LSTCOMP_HXX
//ASDBG #include <usr/iterhlp.hxx>
//ASDBG #endif
//ASDBG #ifndef _USR_SERVICES_HXX
//ASDBG #include <usr/services.hxx>
//ASDBG #endif
#include <com/sun/star/chaos/XCommandExecutor.hpp>
#include <com/sun/star/chaos/XStatusCallback.hpp>
#include <com/sun/star/chaos/UnknownCommandException.hpp>
#include <com/sun/star/chaos/StatusCallbackEvent.hpp>
#include <com/sun/star/chaos/XURLContent.hpp>
#include <com/sun/star/chaos/XExplorerExchange.hpp>
#include <com/sun/star/beans/PropertyValues.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/PropertyState.hpp>
#include <com/sun/star/beans/XPropertySetInfo.hpp>
#include <com/sun/star/beans/XMultiPropertySet.hpp>
#include <com/sun/star/beans/XFastPropertySet.hpp>
#include <com/sun/star/beans/XVetoableChangeListener.hpp>
#include <com/sun/star/beans/XPropertyState.hpp>
#include <com/sun/star/beans/XPropertyStateChangeListener.hpp>
#include <com/sun/star/beans/PropertyAttribute.hpp>
#include <com/sun/star/beans/XPropertiesChangeListener.hpp>
#include <com/sun/star/beans/XPropertyChangeListener.hpp>
#include <com/sun/star/beans/XPropertyAccess.hpp>
#include <com/sun/star/beans/XPropertyContainer.hpp>
#include <com/sun/star/beans/PropertyStateChangeEvent.hpp>
#include <com/sun/star/beans/PropertyChangeEvent.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <cppuhelper/weak.hxx>
#include <cppuhelper/interfacecontainer.hxx>

class CntAnchor;

class TplAnchorListener;

// -----------------
// - TplComponent -
// -----------------

class TplComponent : public ::com::sun::star::chaos::XURLContent, public ::com::sun::star::chaos::XCommandExecutor,
					 public ::com::sun::star::beans::XPropertySet, public ::com::sun::star::chaos::XExplorerExchange,
					 public ::cppu::OWeakObject, public ::com::sun::star::lang::XServiceInfo
{
private:

	void						HandleNewEvt( const ::rtl::OUString& rCommand,
											  const ::com::sun::star::uno::Any& rAny );
	void						CreatePopupMenu( ::com::sun::star::uno::Any& rAny ) const;

protected:

	::rtl::OUString						maURL;
	::rtl::OUString						maName;
    ::osl::Mutex         aMutex;
    ::cppu::OInterfaceContainerHelper maListeners;
	TplAnchorListener*			mpListener;
	short						mnOpenMode;

public:

	SFX_DECL_XINTERFACE_XTYPEPROVIDER_XSERVICEINFO

								TplComponent();
								~TplComponent();

	// interface  ::com::sun::star::ucb::XContent
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >  SAL_CALL		getParent() throw ( ::com::sun::star::uno::RuntimeException )
			{ ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >  aRef; return aRef; }
	virtual void	    SAL_CALL				setParent( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > & Parent ) throw ( ::com::sun::star::uno::RuntimeException )
			{}

	// interface  ::com::sun::star::chaos::XURLContent
	virtual ::rtl::OUString	  SAL_CALL				getURL()  throw ( ::com::sun::star::uno::RuntimeException )
			{ return maURL; }
	virtual sal_Bool	   SAL_CALL				initialize( const ::com::sun::star::uno::Reference< ::com::sun::star::chaos::XURLContent > & aParent, const ::rtl::OUString& aURL ) throw ( ::com::sun::star::uno::RuntimeException );

	// interface  ::com::sun::star::chaos::XCommandExecutor
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >  SAL_CALL	 queryCommands()  throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	   SAL_CALL				cancel( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > & rCommandId ) throw ( ::com::sun::star::uno::RuntimeException )
			{}
	virtual void	   SAL_CALL				execute( const ::rtl::OUString& rCommand, const ::com::sun::star::uno::Any& rArgs,
										 const ::com::sun::star::uno::Reference< ::com::sun::star::chaos::XStatusCallback > & rCallback ) throw ( ::com::sun::star::uno::RuntimeException );

	// interface  ::com::sun::star::beans::XPropertySet
	virtual	::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo >  SAL_CALL	 getPropertySetInfo()  throw ( ::com::sun::star::uno::RuntimeException );
    virtual void      SAL_CALL	           setPropertyValue( const ::rtl::OUString& aPropertyName, const ::com::sun::star::uno::Any& aValue ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual ::com::sun::star::uno::Any	 SAL_CALL				getPropertyValue( const ::rtl::OUString& aPropertyName )  throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	  SAL_CALL				addPropertyChangeListener( const ::rtl::OUString& aPropertyName,
                                                           const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener > & aListener ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	  SAL_CALL				removePropertyChangeListener( const ::rtl::OUString& aPropertyName,
                                                              const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener > & aListener ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	  SAL_CALL				addVetoableChangeListener( const ::rtl::OUString& aPropertyName,
                                                           const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XVetoableChangeListener > & aListener ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	  SAL_CALL				removeVetoableChangeListener( const ::rtl::OUString& aPropertyName,
                                                              const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XVetoableChangeListener > & aListener ) throw ( ::com::sun::star::uno::RuntimeException );
	// interface  ::com::sun::star::chaos::XExplorerExchange
	virtual ::com::sun::star::uno::Any	   SAL_CALL				createDataObject( sal_uInt16& nSourceOptions ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual sal_uInt16	  SAL_CALL				insertDataObject( const ::com::sun::star::uno::Any& pSotDataObjectPtr,
												  sal_uInt16 nUserAction,
												  sal_uInt16 nSourceOptions ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual sal_Bool	  SAL_CALL				queryInsertDataObject( const ::com::sun::star::uno::Any& pSotDataObjectPtr,
													   sal_uInt16 nUserAction,
													   sal_uInt16 nSourceOptions,
													   sal_uInt16& nResultingAction,
													   sal_uInt16& nDefaultAction ) throw ( ::com::sun::star::uno::RuntimeException );


	// -------------------------------------------------------------

	void						CreateNewChild( CntAnchor* pAnchor,
												const String& rTitle );
	void						RemoveChild( CntAnchor* pAnchor );
	void						StopListening( CntAnchor* pAnchor );
	void						HandleOpenEvt();
};

// ------------------------
// - TplContentsComponent -
// ------------------------

class TplContentsListener;

class TplContentsComponent : public TplComponent
{
private:
	::com::sun::star::uno::Reference< ::com::sun::star::chaos::XURLContent > 				_xParent;
	TplContentsListener*		_pListener;
	SvDataObjectRef				_xObject;
	sal_uInt16						_nLayout;

	void						HandleOpenEvt( const ::com::sun::star::uno::Reference< ::com::sun::star::chaos::XStatusCallback > & rJob,
											   const ::com::sun::star::chaos::StatusCallbackEvent& rEvt );
	void						HandleNewEvt( const ::rtl::OUString& rCommand,
											  const ::com::sun::star::uno::Any& rAny );
	void						HandleInitEvt();

	void						CreateFileName( String& rName,
												String& rDirURL );
	void						CreateNewTpl( const String& rFilter,
											  const String& rParentDir,
											  const String& rFileName,
											  const String& rDocName );
protected:

	void						CreatePopupMenu( ::com::sun::star::uno::Any& rAny ) const;

public:

	SFX_DECL_XINTERFACE_XTYPEPROVIDER_XSERVICEINFO

								TplContentsComponent();
								~TplContentsComponent();

	virtual sal_Bool   SAL_CALL				initialize( const ::com::sun::star::uno::Reference< ::com::sun::star::chaos::XURLContent > & aParent, const ::rtl::OUString& aURL ) throw ( ::com::sun::star::uno::RuntimeException );


	// interface  ::com::sun::star::chaos::XCommandExecutor
	virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >  SAL_CALL	 queryCommands()  throw ( ::com::sun::star::uno::RuntimeException );
	virtual void		 SAL_CALL			execute( const ::rtl::OUString& rCommand, const ::com::sun::star::uno::Any& rArgs,
										 const ::com::sun::star::uno::Reference< ::com::sun::star::chaos::XStatusCallback > & rCallback ) throw ( ::com::sun::star::uno::RuntimeException );

	// interface  ::com::sun::star::beans::XPropertySet
	virtual	::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo >  SAL_CALL	 getPropertySetInfo()  throw ( ::com::sun::star::uno::RuntimeException );
	virtual ::com::sun::star::uno::Any	  SAL_CALL				getPropertyValue( const ::rtl::OUString& aPropertyName )  throw ( ::com::sun::star::uno::RuntimeException );
	virtual void	   SAL_CALL				setPropertyValue( const ::rtl::OUString& aPropertyName, const ::com::sun::star::uno::Any& aValue ) throw ( ::com::sun::star::uno::RuntimeException );

	// interface  ::com::sun::star::chaos::XExplorerExchange
	virtual ::com::sun::star::uno::Any	    SAL_CALL				createDataObject( sal_uInt16& nSourceOptions ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual sal_uInt16  SAL_CALL					insertDataObject( const ::com::sun::star::uno::Any& pSotDataObjectPtr,
												  sal_uInt16 nUserAction,
												  sal_uInt16 nSourceOptions ) throw ( ::com::sun::star::uno::RuntimeException );
	virtual sal_Bool    SAL_CALL					queryInsertDataObject( const ::com::sun::star::uno::Any& pSotDataObjectPtr,
													   sal_uInt16 nUserAction,
													   sal_uInt16 nSourceOptions,
													   sal_uInt16& nResultingAction,
													   sal_uInt16& nDefaultAction ) throw ( ::com::sun::star::uno::RuntimeException );

	// -------------------------------------------------------------

	void						CreateNewChild( CntAnchor* pAnchor );
	void						HandleDeleteEvt();
	void						SetURL( const ::rtl::OUString& rURL);
	void						SetName( const ::rtl::OUString& rNewName );
};


#endif

