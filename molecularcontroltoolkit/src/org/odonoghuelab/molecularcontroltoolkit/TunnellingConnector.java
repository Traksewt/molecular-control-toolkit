/**
 * 
 */
package org.odonoghuelab.molecularcontroltoolkit;

import org.odonoghuelab.molecularcontroltoolkit.internal.ConnectorInternal;

/**
 * @author kennethsabir
 *
 */
public interface TunnellingConnector extends Connector {
	public void tunnel(String method, float[] args);

}
